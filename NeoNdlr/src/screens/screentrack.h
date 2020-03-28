#ifndef _SCREENTRACK_H
#define _SCREENTRACK_H

#include "ui/uiscreen.h"
#include "ndlr/ndlr.h"
#include "ndlr/track.h"

class ScreenTrack : public UIScreen {
    bool _needsUpdate;
    Ndlr* _ndlr;
    uint8_t _trackIdx;
    bool _shift;
public:
    ScreenTrack( Ndlr* ndlr, uint8_t trackIdx);
    virtual void render();
    virtual UIEvent* onIndexPressed( uint8_t index);
    virtual UIEvent* onIndexReleased( uint8_t index);
    virtual UIEvent* onEncoderChanged( uint8_t index, int8_t value);
    virtual void forceUpdate();
};

#endif