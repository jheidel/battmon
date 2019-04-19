#ifndef BATTMON_ADC_H_
#define BATTMON_ADC_H_

#include "task.h"

class Adc : public Task {
public:
  bool Setup() override;
  void Run() override;
};

#endif // BATTMON_ADC_H_
