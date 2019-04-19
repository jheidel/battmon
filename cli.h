#ifndef BATTMON_CLI_H_
#define BATTMON_CLI_H_

#include "task.h"
#include <Arduino.h>

class Cli : public Task {
public:
  bool Setup() override;
  bool CanRun() override;
  void Run() override;

  enum State {
    STATE_CMD_SELECT = 0,
    STATE_CAL_0,
    STATE_CAL_1,
  };

private:
  void RunCmdSelect(const char* cmd, size_t cmd_size);
  void RunCal0(const char* cmd, size_t cmd_size);
  void RunCal1(const char* cmd, size_t cmd_size);

  State state_;
};

#endif // BATTMON_CLI_H_
