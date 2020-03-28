#ifndef _CHORD_H
#define _CHORD_H

#include "ndlr/note.h"


class Chord {
    uint8_t _length;
    Note** _notes;
public:
    Chord( Note* note) {
        _length = 1;
        _notes = new Note*[1];
        _notes[0] = note;
    }
    Chord( uint8_t length) {
        _notes = new Note*[length];
        _length = length;
        for( uint8_t i = 0; i < length; ++i) {
            _notes[i] = 0;
        }
    }
    void setNote( uint8_t idx, Note* note) {
        if( idx >= _length) {
            return;
        }
        _notes[idx] = note;
    }
    uint8_t getLength() {
        return _length;
    }
    Note* getNote( uint8_t idx) {
        if( idx >= _length) {
            return 0;
        }
        return _notes[idx];
    }

};

#endif