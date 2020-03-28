#ifndef _NOTE_H
#define _NOTE_H

#include <Arduino.h>

struct Note {
    uint8_t value;
    uint8_t velocity;
    uint8_t length;
    bool legato;

    Note( uint8_t va, uint8_t ve, uint8_t l, bool le) {
        value = va;
        velocity = ve;
        length = l;
        legato = le;
    }
};

#endif