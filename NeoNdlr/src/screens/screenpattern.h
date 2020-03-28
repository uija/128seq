#ifndef _SCREENPATTERN_H
#define _SCREENPATTERN_H

#include <Arduino.h>
#include "ui/uiscreen.h"
#include "ui/uievent.h"
#include "ndlr/ndlr.h"
#include "ndlr/generator.h"

class ScreenPattern : public UIScreen {
protected:
    bool _needsUpdate;
    Ndlr* _ndlr;
    uint8_t _trackIdx;
    uint8_t _patternIdx;
    uint8_t _page;
    uint8_t _selectedGenerator;
    virtual void renderInternal( Generator* gen) { };
    virtual uint8_t getPageCount() { return 1; }

public:
    ScreenPattern( Ndlr* ndlr, uint8_t trackIdx, uint8_t patternIdx);
    virtual void render();
    virtual UIEvent* onIndexPressed( uint8_t index);
    virtual UIEvent* onIndexReleased( uint8_t index);
    virtual UIEvent* onEncoderChanged( uint8_t index, int8_t value);
    virtual void forceUpdate();
};

#endif