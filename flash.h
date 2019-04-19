#ifndef BATTMON_FLASH
#define BATTMON_FLASh

#include "globals.h"
#include "task.h"

class Flasher : public Task {
public:
  Flasher(unsigned long on, unsigned long off) : on_(on), off_(off) {
    state_ = false;
  }

  bool CanRun() override { return true; }

  void Run() override {
    if (state_) {
      state_ = false;
      digitalWrite(LED_BUILTIN, LOW);
      set_interval(off_);
    } else {
      state_ = true;
      digitalWrite(LED_BUILTIN, HIGH);
      set_interval(on_);
    }
  }

private:
  unsigned long on_;
  unsigned long off_;
  bool state_;
};

#endif // BATTMON_FLASH
