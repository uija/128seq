#ifndef _SCREENPATTERNSINGLE_H
#define _SCREENPATTERNSINGLE_H

#include "ui/uiscreen.h"
#include "screens/screenpatterneuclidean.h"

class ScreenPatternSingle : public ScreenPatternEuclidean {
    virtual void renderInternal( Generator* gen);
    virtual uint8_t getPageCount();
public:
    ScreenPatternSingle( Ndlr* ndlr, uint8_t trackIdx, uint8_t patternIdx);
    virtual UIEvent* onEncoderChanged( uint8_t index, int8_t value);
};

#endif