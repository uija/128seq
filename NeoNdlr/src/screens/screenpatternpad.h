#ifndef _SCREENPATTERNPAD_H
#define _SCREENPATTERNPAD_H

#include "screens/screenpatterneuclidean.h"

class ScreenPatternPad : public ScreenPatternEuclidean {
    virtual void renderInternal( Generator* gen);
    virtual uint8_t getPageCount();
public:
    ScreenPatternPad( Ndlr* ndlr, uint8_t trackIdx, uint8_t patternIdx);
    virtual UIEvent* onEncoderChanged( uint8_t index, int8_t value);
};

#endif