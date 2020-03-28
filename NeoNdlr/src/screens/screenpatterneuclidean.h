#ifndef _SCREENPATTERNEUCLIDEAN_H
#define _SCREENPATTERNEUCLIDEAN_H

#include <Arduino.h>
#include "screens/screenpattern.h"
#include "ndlr/ndlr.h"

class ScreenPatternEuclidean : public ScreenPattern {
protected:
    virtual void renderInternal( Generator* gen);
    virtual uint8_t getPageCount();
public:
    ScreenPatternEuclidean( Ndlr* ndlr, uint8_t trackIdx, uint8_t patternIdx);
    virtual UIEvent* onIndexPressed( uint8_t index);
    virtual UIEvent* onEncoderChanged( uint8_t index, int8_t value);
};

#endif