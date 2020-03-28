#ifndef _SCREENPATTERNFIXED_H
#define _SCREENPATTERNFIXED_H

#include "ui/uiscreen.h"
#include "screens/screenpatterneuclidean.h"

class ScreenPatternFixed : public ScreenPatternEuclidean {
    virtual void renderInternal( Generator* gen);
    virtual uint8_t getPageCount();
public:
    ScreenPatternFixed( Ndlr* ndlr, uint8_t trackIdx, uint8_t patternIdx);
    virtual UIEvent* onEncoderChanged( uint8_t index, int8_t value);
};

#endif