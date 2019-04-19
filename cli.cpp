#include "cli.h"

#include "adc.h"
#include <Arduino.h>

bool Cli::Setup() {
  Serial.begin(9600);
  Serial.println(F("-- BattMon 1.0 --"));
  Serial.println(F("Enter \"h\" for help."));
}

bool Cli::CanRun() { return Serial.available() > 0; }

void Cli::Run() {
  // Read input command.
  char buf[8] = {0};
  int read_len = Serial.readBytesUntil('\n', buf, sizeof(buf));

  // Consume any excess characters.
  while (Serial.available()) {
    Serial.read();
  }

  // Echo back characters.
  Serial.print(F("> "));
  Serial.println(buf);

  switch (state_) {
  case STATE_CMD_SELECT:
    RunCmdSelect(buf, read_len);
    break;
  case STATE_CAL_0:
    RunCal0(buf, read_len);
    break;
  case STATE_CAL_1:
    RunCal1(buf, read_len);
    break;
  case STATE_CAL_2:
    RunCal2(buf, read_len);
    break;
  }
}

void Cli::RunCmdSelect(const char* cmd, size_t cmd_size) {
  // Handle commands.
  if (cmd_size != 1) {
    Serial.println(F("Invalid command. Enter \"h\" for help."));
    return;
  }
  switch (cmd[0]) {
  case 'c':
    Serial.println(F("Starting ADC channel calibration."));
    Serial.println(F("Which channel [1-8]?"));
    state_ = STATE_CAL_0;
    break;

  case 'h':
    Serial.println(F("Valid commands:"));
    Serial.println(F("\t'c' - start adc channel calibration"));
    Serial.println(F("\t'h' - this screen"));
    break;

  default:
    Serial.print(F("Unknown command. \""));
    Serial.print(cmd[0]);
    Serial.println(F("\". Enter \"h\" for help."));
    break;
  }
}

namespace {

// ADC calibration locals.
uint8_t adc_channel;
uint16_t adc_mv_0;
uint16_t adc_mv_1;
unsigned long adc_reading_0;
unsigned long adc_reading_1;

uint16_t ReadInputVoltage(const char* cmd, size_t cmd_size) {
  int mv = atoi(cmd);
  if (mv <= 0 || mv > 30000) {
    Serial.println(F("Voltage out of range."));
    return 0;
  }

  uint16_t ret = mv;
  Serial.print(F("Input voltage "));
  char buf[8];
  snprintf(buf, 8, "%u", ret);
  Serial.print(buf);
  Serial.println(F(" mv"));
  return ret;
}

unsigned long Sample() {
  Serial.println(F("Sampling..."));
  const int samples = 10;
  unsigned long acc = 0;
  for (int i = 0; i < samples; ++i) {
    acc += ReadAdcChannel(adc_channel);
    delay(100);
  }
  acc /= samples;
  Serial.println(F("Sampling complete."));
  return acc;
}

} // namespace

void Cli::RunCal0(const char* cmd, size_t cmd_size) {
  int chan = atoi(cmd);
  if (chan <= 0 || chan > 8) {
    Serial.println(F("Channel out of range."));
    state_ = STATE_CMD_SELECT;
    return;
  }
  adc_channel = chan - 1;

  char buf[8];
  Serial.print(F("Selected ADC channel "));
  snprintf(buf, 8, "%u", adc_channel + 1);
  Serial.print(buf);
  Serial.println();

  Serial.println(F("Adjust the supply to a low voltage."));
  Serial.println(F("What's the voltage for sample #1 (mV)?"));
  state_ = STATE_CAL_1;
}

void Cli::RunCal1(const char* cmd, size_t cmd_size) {
  adc_mv_0 = ReadInputVoltage(cmd, cmd_size);
  if (!adc_mv_0) {
    state_ = STATE_CMD_SELECT;
    return;
  }
  adc_reading_0 = Sample();

  Serial.println(F("Adjust the supply to a high voltage."));
  Serial.println(F("What's the voltage for sample #2 (mV)?"));
  state_ = STATE_CAL_2;
}

void Cli::RunCal2(const char* cmd, size_t cmd_size) {
  adc_mv_1 = ReadInputVoltage(cmd, cmd_size);
  if (!adc_mv_1) {
    state_ = STATE_CMD_SELECT;
    return;
  }
  adc_reading_1 = Sample();

  // Compute ADC accuracy...
  double a1 = double(adc_reading_0);
  double c1 = double(adc_mv_0) * 32768 / 6144;
  double a2 = double(adc_reading_1);
  double c2 = double(adc_mv_1) * 32768 / 6144;

  // Cramer's rule solve for scale and offset.
  double m = (c1 - c2) / (a1 - a2);
  double b = (a1 * c2 - c1 * a2) / (a1 - a2);

  Serial.println(F("Computed scaling m="));
  char buf[8];
  dtostrf(m, 0, 3, buf);
  Serial.print(buf);
  Serial.println();

  Serial.println(F("Computed offset b="));
  dtostrf(b, 0, 1, buf);
  Serial.print(buf);
  Serial.println();

  // Solve for map calibration values.
  // TODO......
  // TODO, above calculation doesn't seem correct.

  Serial.println(F("Calibration successful."));
  state_ = STATE_CMD_SELECT;
}
