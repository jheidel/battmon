#include "cli.h"

#include <Arduino.h>

namespace {} // namespace

bool Cli::Setup() {
  Serial.begin(9600);
  Serial.println(F("-- BattMon 1.0 --"));
  Serial.println(F("Enter \"h\" for help."));
}

bool Cli::CanRun() { return Serial.available() > 0; }

void Cli::Run() {
  // Read input command.
  char buf[8];
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

void Cli::RunCal0(const char* cmd, size_t cmd_size) {

  int chan = atoi(cmd);
  if (chan <= 0 || chan > 8) {
    Serial.println(F("Couldn't parse that."));
    state_ = STATE_CMD_SELECT;
    return;
  }

  // TODO something useful.

  Serial.println(F("That's this many:"));
  for (int i = 0; i < chan; ++i) {
    Serial.print(F("@"));
  }
  Serial.println("");

  Serial.println(F("Cool, what's the target voltage?"));

  state_ = STATE_CAL_1;
}

void Cli::RunCal1(const char* cmd, size_t cmd_size) {
  Serial.println(F("Excellent, we're all done here."));
  state_ = STATE_CMD_SELECT;
}
