#include "display.h"

#include "globals.h"
#include "settings.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

namespace {

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

inline void PrintCells(char* buf) {
  char mvb[8];

  display.setCursor(0, display.height() / 2);

  for (int i = 0; i < channels_count; ++i) {
    dtostrf(float(settings.volt_display == DISPLAY_CELLS ? cells_mv[i]
                                                         : channels_mv[i]) /
                1000,
            5, 3, mvb);
    sprintf(buf, "%c%d: %s", settings.volt_display == DISPLAY_CELLS ? 'C' : 'V',
            i + 1, mvb);

    if (i % 2 == 0) {
      display.print(buf);
    } else {
      display.setCursor(display.width() / 2, display.getCursorY());
      display.println(buf);
    }
  }
}

inline void DrawStatusScreen() {
  char buf[9] = {0};

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println(F("System Uptime"));

  unsigned long up_sec = millis() / 1000;
  unsigned long up_min = up_sec / 60;
  unsigned long up_hour = up_min / 60;
  up_min -= up_hour * 60;
  up_sec -= up_hour * 60 + up_min * 60;
  snprintf(buf, sizeof(buf), "%lu:%02lu:%02lu", up_hour, up_min, up_sec);

  display.setCursor(0, display.getCursorY() + 2);
  display.setTextSize(2);
  display.println(buf);

  display.setTextSize(1);
  PrintCells(buf);
  display.display();
}

inline void DrawSettingsScreen() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println(F("Settings"));
  display.display();
}

} // namespace

bool Display::Setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, ADDR_OLED)) {
    return false;
  }
  return true;
}

void Display::Run() {
  switch (runtime.screen_state) {
  case 0:
    DrawStatusScreen();
    break;
  case 1:
    DrawSettingsScreen();
    break;
  }
}
