#ifndef BATTMON_DISPLAY_H_
#define BATTMON_DISPLAY_H_

#include "task.h"

class Display : public Task {
public:
  bool CanRun() override;
  bool Setup() override;
  void Run() override;
};

void ScreenIncrement();
void ScreenSelect();

#endif // BATTMON_DISPLAY_H_
