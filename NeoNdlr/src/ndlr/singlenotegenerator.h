#ifndef _SINGLENOTEGENERATOR_H
#define _SINGLENOTEGENERATOR_H

#include "ndlr/euclideangenerator.h"

class SingleNoteGenerator : public EuclideanGenerator {
    bool _legato;
    int8_t _intervaloffset;
protected:
    Chord* getRealStepChord( uint16_t step, Progression* progression);
public:
    SingleNoteGenerator();
    SingleNoteGenerator( int8_t offset);
    bool isLegato();
    int8_t getIntervalOffset();
    void setIntervalOffset( int8_t io);
    void setLegato( bool legato);
};

#endif