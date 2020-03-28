#include "progression.h"

Progression::Progression() {
    _root = 60;
    _scale = &scales[0];
    _progression = &progressionIntervals[1];
    _progressionStep = 0;
    _index = 0;
}
void Progression::setRootNote( uint8_t root) {
    _root = root;
}
uint8_t Progression::getRootNote() {
    return _root;
}
uint8_t Progression::getCurrentNote( int8_t offset) {
    return _root + _scale->intervals[_index] + offset;
}
uint8_t Progression::getCurrentIndex() {
    return _index;
}
void Progression::setCurrentIndex( uint8_t index) {
    _index = index;
}
void Progression::nextStep() {
    _progressionStep++;
    if( _progressionStep >= _progression->size) {
        _progressionStep = 0;
    }
    _index = _progression->intervals[_progressionStep];
}
Chord* Progression::getCurrentChord( int8_t offset, uint8_t velocity, uint8_t length, bool legato) {
    Chord* chord = new Chord( 3);
    for( uint8_t i = 0; i < 3; ++i) {
        if( _scale->chords[_index] == 0) {
            chord->setNote( i, new Note( _root + CHORD_INTERVALS_MINOR.data[i] + offset, velocity, length, legato));
        }
    }
    return chord;
}