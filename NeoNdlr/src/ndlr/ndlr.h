#ifndef _NDLR_H
#define _NDLR_H

#include <Arduino.h>
#include "ndlr/track.h"
#include "ndlr/progression.h"
#include "ndlr/progressionpattern.h"

#define DEFAULT_BPM 120
#define DEFAULT_SYNC 16
#define SYNC_DIVIDER 6

#define BPMTIME 6000000
#define BPMTIMEFACTOR 24

#define TRACKCOUNT 8
#define PATTERNCOUNT 7

class Ndlr {
    uint8_t _bpm;
    uint16_t _interval;
    uint16_t _intervalCount;
    bool _running;
    uint8_t _syncCount;
    uint8_t _syncValue;
    uint8_t _syncInterval;

    Track _tracks[TRACKCOUNT];

    int8_t _patternIndex[TRACKCOUNT];
    int8_t _patternIndexQueue[TRACKCOUNT];
    bool _muteQueue[TRACKCOUNT];
    uint8_t _progressionIndexQueue;
    uint8_t _progressionPatternIndex;
    uint8_t _progressionPatternIndexQueue;

    Generator** _generators;
    ProgressionPattern** _progressionPatterns;
    Progression _progression;

    bool onClock();
public:
    Ndlr();
    ~Ndlr();
    void setBpm( uint8_t bpm);
    uint8_t getBpm();
    bool isRunning();
    Generator* getGenerator( uint8_t trackIdx, uint8_t patternIdx);
    bool setGenerator( uint8_t trackIdx, uint8_t patternIdx, Generator* generator);
    Progression* getProgression();
    void queueNextProgressionIndex( uint8_t index);
    Track* getTrack( uint8_t idx);
    int8_t getActivePattern( uint8_t trackIdx);
    int8_t getQueuedPattern( uint8_t trackIdx);
    void setActivePattern( uint8_t trackIdx, int8_t patternIdx);
    void setActivePattern( uint8_t trackIdx, int8_t patternIdx, bool instant);
    void toggle();
    bool tick();
    void setSyncValue( uint8_t syncValue);
    uint8_t getSyncValue();
    uint8_t getSyncCount();
    void toggle( uint8_t trackIdx, bool instant);
    bool getMuteQueueValue( uint8_t trackIdx);
    ProgressionPattern* getProgressionPatternAt( uint8_t idx);
    uint8_t getProgressionPatternIndex();
    uint8_t getProgressionPatternQueuedIndex();
    void queueProgressionPatternIndex( uint8_t index);
};

#endif