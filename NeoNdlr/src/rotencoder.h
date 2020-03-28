#ifndef _ENCODER_H
#define _ENCODER_H

#include <Arduino.h>

class RotEncoder {
uint8_t _pinClk;
uint8_t _pinDt;
short _clk;
short _dt;
bool _handled;
public:
    RotEncoder( uint8_t clk, uint8_t dt);
    void setup();
    short tick();
};

#endif
