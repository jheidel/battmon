#include "button.h"

#include <Arduino.h>

#include "display.h"
#include "globals.h"
#include "settings.h"

namespace {

#define DEBOUNCE_MS 100

volatile byte button_0 = LOW;
volatile byte button_1 = LOW;

void IsrButton0() { button_0 = HIGH; }
void IsrButton1() { button_1 = HIGH; }

} // namespace

bool Buttons::Setup() {
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_0), IsrButton0, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_1), IsrButton1, RISING);
  set_interval(100);
  return true;
}

bool Buttons::CanRun() {
  return button_0 == HIGH || button_1 == HIGH;
}

void Buttons::Run() {
  if (is_first_run()) {
    set_interval(0);
    button_0 = LOW;
    button_1 = LOW;
  }

  if (button_0 == HIGH) {
    Press0();
    button_0 = LOW;
  }
  if (button_1 == HIGH) {
    Press1();
    button_1 = LOW;
  }
}

void Buttons::Press0() {
  tone(BUZZER_PIN, 1046, DEBOUNCE_MS / 2);
  delay(DEBOUNCE_MS);

  ScreenIncrement();
}

void Buttons::Press1() {
  tone(BUZZER_PIN, 1046, DEBOUNCE_MS / 2);
  delay(DEBOUNCE_MS);

  ScreenSelect();
}
