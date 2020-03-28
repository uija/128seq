#ifndef _FIXED_NOTE_GENERATOR_H
#define _FIXED_NOTE_GENERATOR_H

#include "ndlr/euclideangenerator.h"

class FixedNoteGenerator : public EuclideanGenerator {
    uint8_t _noteValue;
protected:
    Chord* getRealStepChord( uint16_t step, Progression* progression);
public:
    FixedNoteGenerator();
    FixedNoteGenerator( uint8_t value);
    uint8_t getNoteValue();
    void setNoteValue( uint8_t value);
};

#endif