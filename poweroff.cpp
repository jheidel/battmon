#include "poweroff.h"

#include "globals.h"
#include "settings.h"

namespace {

void Shutdown() {
  for (int i = 0; i < 5; ++i) {
    digitalWrite(LED_BUILTIN, HIGH);
    tone(BUZZER_PIN, 440 * 2, 300);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
  digitalWrite(PWR_CONTROL, HIGH);
  for (;;) {
  }
}

bool VoltageCritical() {
  if (settings.cutoff_idx == 0) {
    return false; // cutoff is disabled
  }
  for (uint8_t i = 0; i < channels_count; ++i) {
    if (cells_mv[i] > 0 && cells_mv[i] < cutoff_mv[settings.cutoff_idx]) {
      return true;
    }
  }
  return false;
}

} // namespace

void Poweroff::Run() {
  if (VoltageCritical()) {
    if (critical_) {
      if (millis() - since_ > CRITICAL_MS) {
        Shutdown();
      }
    } else {
      critical_ = true;
      since_ = millis();
    }
  } else {
    critical_ = false;
  }
}
