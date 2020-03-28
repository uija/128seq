#include "ndlr/progressionpattern.h"

ProgressionPattern::ProgressionPattern() {
    _step = 0;
    _counter = 0;
}

int8_t ProgressionPattern::tick() {
    int8_t ret = -1;
    if( _counter == 0 && _steps[_step].duration > 0) {
        ret = _steps[_step].interval;
    }
    _counter++;
    if( _counter >= _steps[_step].duration) {
        _counter = 0;
        _step++;
        if( _step >= PROGRESSION_STEP_LENGTH || _steps[_step].duration == 0) {
            _step = 0;
        }
    }
    return ret;
}
void ProgressionPattern::reset() {
    _counter = 0;
    _step = 0;
}
uint8_t ProgressionPattern::getIntervalAt( uint8_t idx) {
    if( idx >= PROGRESSION_STEP_LENGTH) return 0;
    return _steps[idx].interval;
}
uint8_t ProgressionPattern::getDurationAt( uint8_t idx) {
    if( idx >= PROGRESSION_STEP_LENGTH) return 0;
    return _steps[idx].duration;
}
void ProgressionPattern::setIntervalAt( uint8_t idx, uint8_t interval) {
    if( idx >= PROGRESSION_STEP_LENGTH) return;
    _steps[idx].interval = interval;
}
void ProgressionPattern::setDurationAt( uint8_t idx, uint8_t duration) {
    if( idx >= PROGRESSION_STEP_LENGTH) return;
    _steps[idx].duration = duration;
}
bool ProgressionPattern::isActive() {
    return _steps[0].duration != 0;
}