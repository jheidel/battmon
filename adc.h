#ifndef BATTMON_ADC_H_
#define BATTMON_ADC_H_

#include "task.h"

#include <Arduino.h>

class Adc : public Task {
public:
  bool Setup() override;
  void Run() override;
};

long ReadAdcChannel(uint8_t channel_idx);

#endif // BATTMON_ADC_H_
