#ifndef BATTMON_SETTINGS_H_
#define BATTMON_SETTINGS_H_

#include <Arduino.h>

#define SETTINGS_MAGIC 0xABCD

struct PersistentSettings {
  PersistentSettings() {
    magic = SETTINGS_MAGIC;
    size = sizeof(PersistentSettings);
  }

  uint16_t magic;
  uint16_t size;

  // ADC calibration values.
  int16_t adc_cal_l[8];
  int16_t adc_cal_h[8];

  bool enable_heartbeat;
  bool display_raw_voltage;
};

struct RuntimeSettings {};

extern PersistentSettings settings;

extern RuntimeSettings runtime;

void LoadSettings();

void PersistSettings();

#endif // BATTMON_SETTINGS_H_
