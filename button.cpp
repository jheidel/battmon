#include "button.h"

#include <Arduino.h>

#include "globals.h"
#include "settings.h"

namespace {

#define DEBOUNCE_MS 75

volatile byte button_0 = LOW;
unsigned long last_0 = 0;

void IsrButton0() { button_0 = HIGH; }

} // namespace

bool Buttons::Setup() {
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_0), IsrButton0, RISING);
  set_interval(100);
  return true;
}

bool Buttons::CanRun() { return button_0 == HIGH; }

void Buttons::Run() {
  if (is_first_run()) {
    set_interval(0);
    button_0 = LOW;
  }

  if (button_0 == HIGH) {
    Press0();
    button_0 = LOW;
  }
}

void Buttons::Press0() {
  tone(BUZZER_PIN, 1046, DEBOUNCE_MS);
  delay(DEBOUNCE_MS); // crude but effective...

  runtime.screen_state++;
  if (runtime.screen_state >= 2) {
    runtime.screen_state = 0;
  }
}
