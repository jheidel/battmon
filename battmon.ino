#include <SPI.h>

#include "globals.h"

#include "adc.h"
#include "cli.h"
#include "display.h"
#include "flash.h"
#include "task.h"

// TODO some better form of status display.
Flasher status(20, 3000);

Adc adc;
Display display;
Cli cli;

Task* all_tasks[] = {&status, &adc, &display, &cli};

uint16_t channels_mv[8] = {0};
uint8_t channels_count = 0;

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
  digitalWrite(LED_CONTROL, LOW);

  for (int i = 0; i < sizeof(all_tasks) / sizeof(Task*); ++i) {
    if (!all_tasks[i]->Setup()) {
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

  // TODO: move
  if (now > 300000 || channels_mv[0] < 8000) {
    for (int i = 0; i < 5; ++i) {
      tone(BUZZER_PIN, 440 * 2, 300);
      delay(500);
    }
    digitalWrite(PWR_CONTROL, HIGH);
    for (;;)
      ;
  }
}
