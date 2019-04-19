#include "display.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "globals.h"

namespace {

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void PrintCells() {
  char buf[32];
  char mvb[10];

  for (int i = 0; i < 8; ++i) {
    dtostrf(float(channels_mv[i]) / 1000, 5, 3, mvb);
    sprintf(buf, "C%d: %s", i + 1, mvb);

    if (i % 2 == 0) {
      display.print(buf);
    } else {
      display.setCursor(display.width() / 2, display.getCursorY());
      display.println(buf);
    }
  }
}

void DrawCellsScreen() {
  char ts[12] = {0};
  double alive = double(millis()) / 1000;
  dtostrf(alive, 0, 2, ts);

  char buf[32] = {0};
  sprintf(buf, "%s sec", ts);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("Alive");
  display.println(buf);


  display.setTextSize(1);

  PrintCells();
  display.display();
}

}  // namespace

bool Display::Setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, ADDR_OLED)) {
      return false;
  }
  return true;
}

void Display::Run() {
    DrawCellsScreen();
}