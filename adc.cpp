#include "adc.h"

#include "globals.h"
#include <Adafruit_ADS1015.h>

namespace {

Adafruit_ADS1115 ads0(ADDR_ADS0);
Adafruit_ADS1115 ads1(ADDR_ADS1);

// Voltage divider values for the cell sensors.
uint16_t rtop[8] = {570, 1, 1, 1, 1, 1, 1, 1};
uint16_t rbot[8] = {100, 1, 1, 1, 1, 1, 1, 1};

// Reading calibration values, determined by linear interpolation over sampled
// data.
int16_t cal_l[8] = {69, 0, 0, 0, 0, 0, 0, 0};
int16_t cal_h[8] = {-526, 0, 0, 0, 0, 0, 0, 0};

void ReadAds(Adafruit_ADS1115* ads, int start, int len, int pos) {
  for (int i = start; i < len; ++i) {
    long val = ads->readADC_SingleEnded(i);
    if (val < 0) {
      val = 0;
    }
    val *= rtop[pos];
    val /= rbot[pos];

    uint16_t mv = map(val, 0 + cal_l[pos], 32768 + cal_h[pos], 0, 6144);
    channels_mv[pos++] = mv;
  }
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
  // Read all 6 cells.
  ReadAds(&ads0, 0, 4, 0);
  ReadAds(&ads1, 0, 2, 4);

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
