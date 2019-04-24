#ifndef BATTMON_FLASH_H_
#define BATTMON_FLASH_H_

#include "globals.h"
#include "settings.h"
#include "task.h"

class Flasher : public Task {
public:
  Flasher(unsigned long on, unsigned long off) : on_(on), off_(off) {
    state_ = false;
  }

  bool CanRun() override { return settings.enable_heartbeat; }

  void Run() override {
    if (state_) {
      state_ = false;
      digitalWrite(LED_BUILTIN, LOW);
      set_interval(off_);
    } else {
      state_ = true;
      analogWrite(LED_BUILTIN, 16); // dim flash
      set_interval(on_);
    }
  }

private:
  unsigned long on_;
  unsigned long off_;
  bool state_;
};

#endif // BATTMON_FLASH_H_
