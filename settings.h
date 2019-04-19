#ifndef BATTMON_SETTINGS_H_
#define BATTMON_SETTINGS_H_

#include <Arduino.h>

#define SETTINGS_MAGIC 0xABCD

struct Settings {
  uint16_t magic;
  uint16_t size;

  // ADC calibration values.
  int16_t adc_cal_l[8];
  int16_t adc_cal_h[8];
};

extern Settings settings;

void LoadSettings();

void PersistSettings();

#endif // BATTMON_SETTINGS_H_