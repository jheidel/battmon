#ifndef BATTMON_GLOBALS
#define BATTMON_GLOBALS

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1015.h>

#define ADDR_OLED 0x3C
#define ADDR_ADS0 0x48
#define ADDR_ADS1 0x49

#define BUZZER_PIN 8

#define PWR_CONTROL 7
#define LED_CONTROL 6



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Adafruit_ADS1115 ads0(ADDR_ADS0);
Adafruit_ADS1115 ads1(ADDR_ADS1);

#endif // BATTMON_GLOBALS
