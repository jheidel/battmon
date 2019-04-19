#ifndef BATTMON_GLOBALS_H_
#define BATTMON_GLOBALS_H_

#include <Arduino.h>

#define ADDR_OLED 0x3C
#define ADDR_ADS0 0x48
#define ADDR_ADS1 0x49

#define BUZZER_PIN 8

#define PWR_CONTROL 7
#define LED_CONTROL 6

extern uint16_t channels_mv[8];

#endif // BATTMON_GLOBALS_H_