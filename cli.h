#ifndef BATTMON_CLI_H_
#define BATTMON_CLI_H_

#include "task.h"

class Cli : public Task {
public:
  bool Setup() override;
  void Run() override;
};

#endif // BATTMON_CLI_H_
