#ifndef _PROGRESSION_H
#define _PROGRESSION_H

#include <Arduino.h>
#include "ndlr/chord.h"

#define SIZE_INTERVALS 2
#define SIZE_PROGRESSIONS 2

struct SizedIntArray {
    uint8_t size;
    uint8_t * data;
    const char* name;
    SizedIntArray( uint8_t s, uint8_t* d, const char* n) {
        size = s; data = d; name = n;
    }
};
struct Scale {
    const char* name;
    uint8_t* intervals;
    uint8_t* chords;
    Scale( const char* n, uint8_t* i, uint8_t* c) {
        name = n;
        intervals = i;
        chords = c;
    }
};
struct ProgressionInterval {
    const char* name;
    uint8_t size;
    uint8_t* intervals;
    ProgressionInterval(const char* n, uint8_t s, uint8_t* i) {
        name = n;
        size = s;
        intervals = i;
    }
};

static Scale scales[] = {
    Scale( "Maj", (uint8_t[]){0,2,4,5,7,9,11,12}, (uint8_t[]){1, 0, 0, 1, 1, 0, 0}),
    Scale( "Maj", (uint8_t[]){0,2,3,5,7,8,10,12}, (uint8_t[]){0, 1, 1, 0, 0, 1, 0}),
};
static ProgressionInterval progressionIntervals[] = {
    ProgressionInterval( "I", 1, (uint8_t[]){0}),
    ProgressionInterval( "I V VI IV", 4, (uint8_t[]){0,4,5,3}),
};
static SizedIntArray CHORD_INTERVALS_MINOR( 3, (uint8_t[]){0, 3, 7}, "Min");
static SizedIntArray CHORD_INTERVALS_MAJOR( 3, (uint8_t[]){0, 4, 7}, "Maj");

class Progression {
    uint8_t _root;
    Scale* _scale;
    ProgressionInterval* _progression;
    uint8_t _index;
    uint8_t _progressionStep;
public:
    Progression();
    void setRootNote( uint8_t root);
    uint8_t getRootNote();
    uint8_t getCurrentNote( int8_t offset);
    uint8_t getCurrentIndex();
    Chord* getCurrentChord( int8_t offset, uint8_t velocity, uint8_t length, bool legato);
    void setCurrentIndex( uint8_t index);
    void nextStep();
};

#endif