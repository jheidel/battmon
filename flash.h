#ifndef BATTMON_FLASH_H_
#define BATTMON_FLASH_H_

#include "globals.h"
#include "poweroff.h"
#include "settings.h"
#include "task.h"

class Flasher : public Task {
public:
  bool CanRun() override { return settings.enable_heartbeat; }

  void Run() override {
    if (state_) {
      state_ = false;
      analogWrite(LED_BUILTIN, 0);
      if (IsCriticalVoltage()) {
        set_interval(500); // fast flash
      } else {
        set_interval(3000); // slow flash
      }
    } else {
      state_ = true;
      if (IsCriticalVoltage()) {
        analogWrite(LED_BUILTIN, 255); // bright flash
      } else {
        analogWrite(LED_BUILTIN, 16); // dim flash
      }
      set_interval(20);
    }
  }

private:
  bool state_ = false;
};

#endif // BATTMON_FLASH_H_
