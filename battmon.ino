#include "adc.h"
#include "button.h"
#include "cli.h"
#include "display.h"
#include "flash.h"
#include "globals.h"
#include "poweroff.h"
#include "settings.h"
#include "task.h"

Flasher status(20, 3000);
Adc adc;
Display display;
Cli cli;
Buttons buttons;
Poweroff poweroff;

Task* all_tasks[] = {
    &buttons, &status, &adc, &buttons, &display, &buttons, &cli, &poweroff,
};

void Fatal(uint8_t code) {
  for (;;) {
    for (int i = 0; i < 10; ++i) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN, LOW);
      delay(50);
    }
    delay(1000);

    for (int i = 0; i < code; ++i) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
    delay(3000);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PWR_CONTROL, OUTPUT);
  pinMode(LED_CONTROL, OUTPUT);
  pinMode(FAN_CONTROL, OUTPUT);
  digitalWrite(LED_CONTROL, LOW);

  LoadSettings();

  for (int i = 0; i < sizeof(all_tasks) / sizeof(Task*); ++i) {
    if (!all_tasks[i]->SetupInit()) {
      Fatal(CODE_SETUP);
    }
  }

  // Start-up success beep.
  tone(BUZZER_PIN, 440 * 2, 100);
}

void loop() {
  unsigned long now = millis();
  for (int i = 0; i < sizeof(all_tasks) / sizeof(Task*); ++i) {
    all_tasks[i]->Tick(now);
  }
}
