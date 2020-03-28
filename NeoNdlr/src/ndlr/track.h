#ifndef _TRACK_H
#define _TRACK_H

#include "ndlr/generator.h"
#include "ndlr/progression.h"

#define POLY_COUNT 16

struct StackNote {
    int16_t value;
    int16_t length;
    uint8_t velocity;
    int16_t lifetime;
    StackNote() {
        value = -1;
        length = -1;
        velocity = 0;
        lifetime = 0;
    }
};

class Track {
    int8_t _midiChannel;
    uint16_t _step;
    bool _muted;
    StackNote _stack[POLY_COUNT];

    void noteOn( uint8_t value, uint8_t velocity);
    void noteOff( uint8_t value, uint8_t velocity);
    void playNote( Note* note);
    void haltAllNotes();
public:
    Track();
    int8_t getMidiChannel();
    void setMidiChannel( int8_t midiChannel);
    void tick( Generator* generator, Progression* progression);
    void stop();
    bool isActive();
    void toggle();
    bool isMuted();
};

#endif