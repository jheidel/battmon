#ifndef BATTMON_SETTINGS_H_
#define BATTMON_SETTINGS_H_

#include <Arduino.h>

#define SETTINGS_MAGIC 0xBBCA

struct PersistentSettings {
  uint16_t magic;
  uint16_t size;

  // ADC calibration values.
  int16_t adc_cal_l[8];
  int16_t adc_cal_h[8];

  bool enable_heartbeat;
  bool display_raw_voltage;
  bool show_sram;

  uint8_t cutoff_idx;
};

extern PersistentSettings settings;

void LoadSettings();

void PersistSettings();

#endif // BATTMON_SETTINGS_H_
