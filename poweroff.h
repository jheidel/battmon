#ifndef BATTMON_POWEROFF_H_
#define BATTMON_POWEROFF_H_

#include "task.h"
#include <Arduino.h>

constexpr uint16_t cutoff_mv[] = {0, 2700, 2800, 2900, 3000, 3100, 3200, 3300};

// Critical cell voltage maintained for this duration will trigger shutdown
#define CRITICAL_MS 30000

class Poweroff : public Task {
public:
  void Run() override;
};

bool IsCriticalVoltage();

#endif // BATTMON_POWEROFF_H_
