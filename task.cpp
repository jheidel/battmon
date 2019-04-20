#include "task.h"

bool Task::SetupInit() {
  if (setup_) {
    return true;
  }
  setup_ = true;
  return Setup();
}

void Task::Tick(unsigned long now) {
  if (interval_ && last_run_ + interval_ > now) {
    return;
  }
  if (!CanRun()) {
    return;
  }
  last_run_ = now;
  Run();
  first_run_ = false;
}
