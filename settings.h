#ifndef BATTMON_SETTINGS_H_
#define BATTMON_SETTINGS_H_

#include <Arduino.h>

#define SETTINGS_MAGIC 0xABCD

enum VoltDisplay {
  DISPLAY_CELLS = 0,
  DISPLAY_VOLTS,
};

struct PersistentSettings {
  uint16_t magic;
  uint16_t size;

  // ADC calibration values.
  int16_t adc_cal_l[8];
  int16_t adc_cal_h[8];

  VoltDisplay volt_display;
};

struct RuntimeSettings {
  uint8_t screen_state = 0;
};

extern PersistentSettings settings;

extern RuntimeSettings runtime;

void LoadSettings();

void PersistSettings();

#endif // BATTMON_SETTINGS_H_
