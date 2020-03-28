#ifndef _EUCLIDEANGENERATOR_H
#define _EUCLIDEANGENERATOR_H

#include "ndlr/euclidean.h"
#include "ndlr/generator.h"

#define DEFAULT_DIVIDER 6
#define DEFAULT_VELOCITY 64

class EuclideanGenerator : public Euclidean, public Generator {
protected:
    uint8_t _noteLength;
    uint8_t _noteVelocity;
    uint8_t _stepDivider;
protected:
    virtual Chord* getRealStepChord( uint16_t step, Progression* progression) = 0;
public:
    EuclideanGenerator( int8_t melody) : Euclidean(), Generator( TYPE_EUCLIDEAN, melody) {
        _noteVelocity = DEFAULT_VELOCITY;
        _stepDivider = DEFAULT_DIVIDER;
        _noteLength = 5;
    }
    uint8_t getNoteVelocity() {
        return _noteVelocity;
    }
    void setNoteVelocity( uint8_t velocity) {
        _noteVelocity = velocity;
    }
    uint8_t getNoteLength() {
        return _noteLength;
    }
    void setNoteLength( uint8_t length) {
        _noteLength = length;
    }
    uint16_t getLength() {
        return this->getPatternLength() * _stepDivider;
    }
    Chord* getChordForStep( uint16_t step, Progression* progression) {
        if( step % _stepDivider != 0) {
            return 0;
        }
        step /= _stepDivider;
        while( step >= this->getPatternLength()) {
            step -= this->getPatternLength();
        }
        if( !this->hasPulse( step)) {
            return 0;
        }
        return this->getRealStepChord( step, progression);
    }
};

#endif
