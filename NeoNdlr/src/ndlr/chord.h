#ifndef _CHORD_H
#define _CHORD_H

#include "ndlr/note.h"

#define MAX_CHORD_SIZE 32

class Chord {
    uint8_t _length;

    uint8_t _noteValue[MAX_CHORD_SIZE];
    uint8_t _noteVelocity[MAX_CHORD_SIZE];
    uint8_t _noteLength[MAX_CHORD_SIZE];
    bool _noteLegato[MAX_CHORD_SIZE];
public:
    Chord( uint8_t value, uint8_t velocity, uint8_t length, bool legato);
    Chord( Note* note);
    Chord( uint8_t length);
    void setNote( uint8_t idx, uint8_t value, uint8_t velocity, uint8_t length, bool legato);
    void setNote( uint8_t idx, Note* note);
    uint8_t getLength();
    Note* getNote( uint8_t idx);
    void rescale( uint8_t size);
    void normalize( uint8_t base);
};

#endif