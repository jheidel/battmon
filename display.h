#ifndef BATTMON_DISPLAY_H_
#define BATTMON_DISPLAY_H_

#include "task.h"

class Display : public Task {
public:
  bool Setup() override;
  void Run() override;
};

#endif // BATTMON_DISPLAY_H_