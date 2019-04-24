#include "display.h"

#include "globals.h"
#include "settings.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
#include "memory.h"

namespace {

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

class Screen {
  public:
    virtual void Draw() = 0;

    virtual void Select() {
      // Default implementation is an error tone.
      tone(BUZZER_PIN, 220, 150);
    }
};

struct Rotation {
  Screen** screens;
  uint8_t length;
};

Rotation* rotation;
uint8_t rotation_idx;


class StatusScreen : public Screen {
  private:

  public:
    void Draw() override {
      char nbuf[9] = {0};

      display.setTextSize(1);
      display.setTextColor(WHITE);

      display.setCursor(0, 0);
      display.println(F("System Uptime"));

      unsigned long up_sec = millis() / 1000;
      unsigned long up_min = up_sec / 60;
      unsigned long up_hour = up_min / 60;
      up_min -= up_hour * 60;
      up_sec -= up_hour * 60 + up_min * 60;

      display.setCursor(0, display.getCursorY() + 1);
      display.setTextSize(2);

      memset(nbuf, 0, sizeof(nbuf));
      snprintf(nbuf, sizeof(nbuf), "%lu:%02lu:%02lu", up_hour, up_min, up_sec);
      display.println(nbuf);

      display.setTextSize(1);

      if (settings.show_sram) {
        display.print(F("Free SRAM: "));
        memset(nbuf, 0, sizeof(nbuf));
        snprintf(nbuf, sizeof(nbuf), "%d", freeMemory());
        display.println(nbuf);
      }

      display.setCursor(0, display.getCursorY() + 2);

      for (int i = 0; i < channels_count; ++i) {
        if (i % 2 != 0) {
          display.setCursor(display.width() / 2, display.getCursorY());
        }
        display.print(settings.display_raw_voltage ? 'V' : 'C');

        memset(nbuf, 0, sizeof(nbuf));
        snprintf(nbuf, sizeof(nbuf), "%d", i + 1);
        display.print(nbuf);
        display.print(F(": "));
        memset(nbuf, 0, sizeof(nbuf));
        dtostrf(
          float(settings.display_raw_voltage ? channels_mv[i] : cells_mv[i]) /
          1000,
          5, 3, nbuf);
        if (i % 2 == 0) {
          display.print(nbuf);
        } else {
          display.println(nbuf);
        }
      }
    }
};

const uint8_t fan_steps[] = {0, 64, 128, 160, 192, 224, 255};

class FanControl : public Screen {
  public:
    void Draw() override {
      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.println(F("Fan"));
      if (fan_steps[fan_idx_] == 0) {
        display.println(F("OFF"));
      } else {
        display.print(F("ON "));
        char buf[8] = {0};
        uint16_t long val = 100 * uint16_t(fan_steps[fan_idx_]) / 255;
        sprintf(buf, "%lu", val);
        display.print(buf);
        display.println(F("%"));
      }
    }

    void Select() override {
      fan_idx_++;
      if (fan_idx_ >= sizeof(fan_steps) / sizeof(uint8_t)) {
        fan_idx_ = 0;
      }
      analogWrite(FAN_CONTROL, fan_steps[fan_idx_]);
    }

    uint8_t fan_idx_ = 0;
};

class MoveRotation : public Screen {
  public:
    MoveRotation(const char* header, const char* help, Rotation* to)
      : header_(header), help_(help), to_(to) {}
    void Draw() override {
      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.println(header_);
      display.setTextSize(1);
      display.println(help_);
    }
    void Select() override {
      rotation = to_;
      rotation_idx = 0;
    }

  private:
    const char* header_;
    const char* help_;
    Rotation* to_;
};

class BoolSetting : public Screen {
  public:
    BoolSetting(const char* name, bool* val) : name_(name), val_(val) {}
    void Draw() override {
      display.setCursor(0, 0);
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.println(F("Settings"));
      display.setTextSize(2);
      display.println(name_);
      display.setTextSize(1);
      display.println(*val_ ? F("ON") : F("OFF"));
    }
    void Select() override {
      *val_ = !*val_;
      PersistSettings();
    }

  private:
    bool* val_;
    const char* name_;
};

unsigned long last_interact = 0;
#define MENU_TIMEOUT 10000
#define SCREEN_TIMEOUT 60000
bool screen_on = true;

// Define the display menu structure.

StatusScreen status;
FanControl fan;
extern Rotation settings_rot;
MoveRotation enter_settings("Settings", "Enter settings menu", &settings_rot);
Screen* primary_screens[] = {&status,
                             &fan,
                             &enter_settings,
                            };
Rotation primary{primary_screens, sizeof(primary_screens) / sizeof(Screen*)};

BoolSetting heartbeat("LED Flash", &settings.enable_heartbeat);
BoolSetting voltage("Show Volts", &settings.display_raw_voltage);
BoolSetting sram("Show SRAM", &settings.show_sram);
MoveRotation settings_back("Back", "Return to status", &primary);
Screen* settings_screens[] = {
  &heartbeat,
  &voltage,
  &sram,
  &settings_back
};
Rotation settings_rot{settings_screens, sizeof(settings_screens) / sizeof(Screen*)};


} // namespace

bool Display::Setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, ADDR_OLED)) {
    return false;
  }
  rotation = &primary;
  rotation_idx = 0;
  screen_on = true;
  return true;
}

bool Display::CanRun() {
  return screen_on;
}

void Display::Run() {
  const unsigned long now = millis();
  if (now > last_interact + MENU_TIMEOUT) {
    // Timeout, return to home screen.
    rotation = &primary;
    rotation_idx = 0;
  }
  if (now > last_interact + SCREEN_TIMEOUT) {
    // Stop displaying on screen.
    display.clearDisplay();
    display.display();
    screen_on = false;
    return;
  }

  display.clearDisplay();
  rotation->screens[rotation_idx]->Draw();
  display.display();
}

void ScreenIncrement() {
  last_interact = millis();
  if (!screen_on) {
    screen_on = true;
    return;
  }

  rotation_idx++;
  if (rotation_idx >= rotation->length) {
    rotation_idx = 0;
  }
}

void ScreenSelect() {
  last_interact = millis();
  if (!screen_on) {
    screen_on = true;
    return;
  }

  rotation->screens[rotation_idx]->Select();
}
