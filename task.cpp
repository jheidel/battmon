#include "task.h"

void Task::Tick(unsigned long now) {
  if (interval_ && last_run_ + interval_ > now) {
    return;
  }
  if (!CanRun()) {
    return;
  }
  last_run_ = now;
  Run();
}
