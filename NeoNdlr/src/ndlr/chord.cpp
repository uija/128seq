#include "ndlr/chord.h"
Chord::Chord( uint8_t value, uint8_t velocity, uint8_t length, bool legato) {
    _length = 1;
    this->setNote( 0, value, velocity, length, legato);
}
Chord::Chord( Note* note) {
    if( note != 0) {
        _length = 1;
        this->setNote( 0, note);
    } else {
        _length = 0;
    }
}
Chord::Chord( uint8_t length) {
    _length = length;
    for( uint8_t i = 0; i < length; ++i) {
        this->setNote( i, 0, 0, 0, false);
    }
}
void Chord::setNote( uint8_t idx, Note* note) {
    if( note != 0) {
        this->setNote( idx, note->value, note->velocity, note->length, note->length);
    } else {
        this->setNote( idx, 0, 0, 0, false);
    }
}
void Chord::setNote( uint8_t idx, uint8_t value, uint8_t velocity, uint8_t length, bool legato) {
    if( idx >= _length) {
        return;
    }
    _noteValue[idx] = value;
    _noteVelocity[idx] = velocity;
    _noteLength[idx] = length;
    _noteLegato[idx] = legato;
}
uint8_t Chord::getLength() {
    return _length;
}
Note* Chord::getNote( uint8_t idx) {
    if( idx >= _length) {
        return 0;
    }
    return new Note( _noteValue[idx], _noteVelocity[idx], _noteLength[idx], _noteLegato[idx]);
}
void Chord::normalize( uint8_t base) {
    if( this->getLength() < 2) return;
    // get the max range of the chord
    uint8_t min = 127;
    uint8_t max = 0;
    for( uint8_t i = 0; i < this->getLength(); ++i) {
        if( _noteValue[i] < min) {
            min = _noteValue[i];
        }
        if( _noteValue[i] > max) {
            max = _noteValue[i];
        }
    }
    uint8_t diff = max - min;
    uint8_t range = (uint8_t)ceil( ((double)diff) / 2.0);
    for( uint8_t i = 0; i < this->getLength(); ++i) {
        while( _noteValue[i] > base + range) {
            _noteValue[i] -= 12;
        }
        while( _noteValue[i] < base - range) {
            _noteValue[i] += 12;
        }
    }
}
void Chord::rescale( uint8_t size) {
    if( size < _length) {
        _length = size;
    } else {
        if( size >= MAX_CHORD_SIZE) {
            size = MAX_CHORD_SIZE;
        }
        for( uint8_t i = _length; i < size; ++i) {
            uint8_t copyIndex = i;
            uint8_t offset = 0;
            while( copyIndex >= _length) {
                copyIndex -= _length;
                offset += 12;
            }
            _noteValue[i] = _noteValue[copyIndex] + offset;
            _noteVelocity[i] = _noteVelocity[copyIndex];
            _noteLength[i] = _noteLength[copyIndex];
            _noteLegato[i] = _noteLegato[copyIndex];
        }
        _length = size;
    }
}