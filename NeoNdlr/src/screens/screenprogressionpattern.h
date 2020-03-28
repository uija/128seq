#ifndef _SCREENPROGRESSIONPATTERN_H
#define _SCREENPROGRESSIONPATTERN_H

#include <Arduino.h>
#include "ui/uiscreen.h"
#include "ui/uievent.h"
#include "screens/screenmain.h"

class ScreenProgressionPattern : public UIScreen {
    Ndlr* _ndlr;
    uint8_t _patternIdx;
    uint8_t _currentStep;
    bool _needsUpdate;
public:
    ScreenProgressionPattern( Ndlr* ndlr, uint8_t patternIdx);
    virtual void render();
    virtual UIEvent* onIndexPressed( uint8_t index);
    virtual UIEvent* onIndexReleased( uint8_t index);
    virtual UIEvent* onEncoderChanged( uint8_t index, int8_t value);
    virtual void forceUpdate();
};

#endif 
