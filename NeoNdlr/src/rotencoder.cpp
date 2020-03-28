#include "rotencoder.h"

RotEncoder::RotEncoder( uint8_t clk, uint8_t dt) {
    _pinClk = clk;
    _pinDt = dt;
    _handled = false;
}
void RotEncoder::setup() {
    pinMode( _pinClk, INPUT);
    pinMode( _pinDt, INPUT);
    digitalWrite( _pinClk, LOW);
    digitalWrite( _pinDt, LOW);
}
short RotEncoder::tick() {
    _clk = digitalRead( _pinClk);
    _dt = digitalRead( _pinDt);
    if( !_handled && _clk == 0 && _dt == 1) {
        _handled = true;
        return 1;
    } else if( !_handled && _clk == 1 && _dt == 0){
        _handled = true;
        return -1;
    } else if( _handled && _clk == 1 && _dt == 1) {
        _handled = false;
    }
    return 0;
}