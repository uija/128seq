#ifndef _GENERATOR_H
#define _GENERATOR_H

#include <Arduino.h>
#include "ndlr/chord.h"
#include "ndlr/progression.h"

#define TYPE_EUCLIDEAN 0
#define TYPE_SEQUENCER 1

#define MELODY_FIXED 0
#define MELODY_SINGLE 1

class Generator {
protected:
    uint8_t _type;
    uint8_t _melody;
public:
    Generator( uint8_t type, uint8_t melody) { _type = type; _melody = melody; }
    virtual ~Generator() {}  
    virtual uint16_t getLength();
    virtual bool needsReset() { return false; }
    virtual Chord* getChordForStep( uint16_t step, Progression* progression); 
    uint8_t getType() { return _type; }
    uint8_t getMelody() { return _melody; }
};

#endif