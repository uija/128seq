#ifndef _PADGENERATOR_H
#define _PADGENERATOR_H

#include <Arduino.h>
#include "ndlr/euclideangenerator.h"

class PadGenerator : public EuclideanGenerator {
    bool _normalize;
protected:
    Chord* getRealStepChord( uint16_t step, Progression* progression);
public:
    PadGenerator();
    bool isNormalize();
    void setNormalize( bool normalize);
};

#endif