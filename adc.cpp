#include "adc.h"

#include "globals.h"
#include "settings.h"
#include <Adafruit_ADS1015.h>

namespace {

Adafruit_ADS1115 ads0(ADDR_ADS0);
Adafruit_ADS1115 ads1(ADDR_ADS1);

// Voltage divider values for the cell sensors.
uint16_t rtop[8] = {570, 570, 570, 570, 570, 570, 1, 1};
uint16_t rbot[8] = {100, 100, 100, 100, 100, 100, 1, 1};

// Reading calibration values, determined by linear interpolation over sampled
// data.
// TODO remove, now in settings.
// int16_t cal_l[8] = {69, 0, 0, 0, 0, 0, 0, 0};
// int16_t cal_h[8] = {-526, 0, 0, 0, 0, 0, 0, 0};

inline void Read(uint8_t idx) {
  long val = ReadAdcChannel(idx);
  uint16_t mv = map(val, 0 + settings.adc_cal_l[idx],
                    32768 + settings.adc_cal_h[idx], 0, 6144);
  channels_mv[idx] = mv;
}

#define SAMPLES_PER_SEC 8

} // namespace

bool Adc::Setup() {
  ads0.setGain(GAIN_TWOTHIRDS);
  ads0.begin();
  ads1.setGain(GAIN_TWOTHIRDS);
  ads1.begin();
}

void Adc::Run() {
  for (uint8_t i = 0; i < 6; ++i) {
    Read(i);
  }

  channels_count = 0;
  for (int i = 0; i < 8; ++i) {
    if (channels_mv[i] < 100) {
      break; // No voltage here!
    }
    channels_count++;
  }

  // TODO the cell delta computations
  // TODO the cell count determination

  set_interval(1000 / SAMPLES_PER_SEC);
}

long ReadAdcChannel(uint8_t channel_idx) {
  if (channel_idx >= 8) {
    return 0; // invalid input
  }
  Adafruit_ADS1115* adc = &ads0;
  if (channel_idx >= 4) {
    adc = &ads1;
    channel_idx -= 4;
  }
  long val = adc->readADC_SingleEnded(channel_idx);
  if (val < 0) {
    val = 0;
  }

  val *= rtop[channel_idx];
  val /= rbot[channel_idx];
  return val;
}
