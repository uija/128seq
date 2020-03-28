#ifndef _SCREENMAIN_H
#define _SCREENMAIN_H

#include "ui/uiscreen.h"
#include "ndlr/ndlr.h"

class ScreenMain : public UIScreen {
    bool _needsUpdate;
    Ndlr* _ndlr;
    bool _shift;
    bool _edit;
public:
    ScreenMain( Ndlr* ndlr);
    virtual void render();
    virtual UIEvent* onIndexPressed( uint8_t index);
    virtual UIEvent* onIndexReleased( uint8_t index);
    virtual UIEvent* onEncoderChanged( uint8_t index, int8_t value);
    virtual void forceUpdate();
};

#endif