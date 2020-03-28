#ifndef _SCREENPATTERNARP_H
#define _SCREENPATTERNARP_H

#include "screens/screenpatterneuclidean.h"

class ScreenPatternArp : public ScreenPatternEuclidean {
    virtual void renderInternal( Generator* gen);
    virtual uint8_t getPageCount();
public:
    ScreenPatternArp( Ndlr* ndlr, uint8_t trackIdx, uint8_t patternIdx);
    virtual UIEvent* onEncoderChanged( uint8_t index, int8_t value);
};

#endif