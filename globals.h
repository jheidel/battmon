#ifndef BATTMON_GLOBALS_H_
#define BATTMON_GLOBALS_H_

#include <Arduino.h>

#define ADDR_OLED 0x3C
#define ADDR_ADS0 0x48
#define ADDR_ADS1 0x49

#define BUZZER_PIN 8

#define PWR_CONTROL 7
#define LED_CONTROL 6
#define FAN_CONTROL 5

#define PIN_BUTTON_0 3
#define PIN_BUTTON_1 2

extern uint16_t channels_mv[8];
extern uint16_t cells_mv[8];
extern uint8_t channels_count;

#define CODE_SETUP 1
void Fatal(uint8_t code);

#endif // BATTMON_GLOBALS_H_
