#include <SPI.h>

#include "globals.h"

#include "task.h"
#include "flash.h"
#include "display.h"
#include "adc.h"
#include "cli.h"

// TODO some better form of status display.
Flasher f1(50, 5000);
Flasher f2(200, 3000);

Adc adc;
Display display;
Cli cli;

Task* all_tasks[] = {&f1, &f2, &adc, &display, &cli};

uint16_t channels_mv[8] = {0};

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PWR_CONTROL, OUTPUT);
  pinMode(LED_CONTROL, OUTPUT);
  digitalWrite(LED_CONTROL, LOW);

  for (int i = 0; i < sizeof(all_tasks) / sizeof(Task*); ++i) {
    if (!all_tasks[i]->Setup()) {
      digitalWrite(LED_BUILTIN, HIGH);
      for (;;); // Don't proceed, loop forever
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
    for (;;);
  }
}
