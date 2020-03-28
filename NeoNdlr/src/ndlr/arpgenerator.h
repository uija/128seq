#ifndef _ARPGENERATOR_H
#define _ARPGENERATOR_H

#include "ndlr/euclideangenerator.h"

class ArpGenerator : public EuclideanGenerator {
    uint8_t _arpIndex;
    uint8_t _arpSize;
    bool _normalize;
protected:
    Chord* getRealStepChord( uint16_t step, Progression* progression);
public:
    ArpGenerator();
    void setArpSize( uint8_t size);
    uint8_t getArpSize();
    virtual void reset();
    bool isNormalize();
    void setNormalize( bool normalize);
};

#endif
