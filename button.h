#ifndef BATTMON_BUTTON_H_
#define BATTMON_BUTTON_H_

#include "task.h"

class Buttons : public Task {
public:
  bool Setup() override;
  bool CanRun() override;
  void Run() override;

private:
  void Press0();
  void Press1();
};

#endif // BATTMON_BUTTON_H_
