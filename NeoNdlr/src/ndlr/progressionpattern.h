#ifndef _PROGRESSIONPATTERN_H
#define _PROGRESSIONPATTERN_H

#include <Arduino.h>
#include "ndlr/progression.h"

#define PROGRESSION_STEP_LENGTH 16

struct ProgressionStep {
  uint8_t interval;
  uint8_t duration;
  ProgressionStep() {
      interval = 0;
      duration = 0;
  }
  ProgressionStep( uint8_t i, uint8_t d) {
      interval = i;
      duration = d;
  }
};

class ProgressionPattern {
ProgressionStep _steps[PROGRESSION_STEP_LENGTH];
uint8_t _step;
uint8_t _counter;
public:
    ProgressionPattern();
    int8_t tick();
    void setStepAt( uint8_t idx, uint8_t interval, uint8_t duration);
    uint8_t getIntervalAt( uint8_t idx);
    uint8_t getDurationAt( uint8_t idx);
    void setIntervalAt( uint8_t idx, uint8_t interval);
    void setDurationAt( uint8_t idx, uint8_t duration);
    
    bool isActive();
    void reset();
};

#endif