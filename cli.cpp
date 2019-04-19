#include "cli.h"

#include <Arduino.h>

bool Cli::Setup() {
  Serial.begin(9600);
  Serial.println("-- BattMon 1.0 --");
  set_interval(3000);
}

void Cli::Run() {
  Serial.println("Ping.");

  // TODO support input for config and things.
}
