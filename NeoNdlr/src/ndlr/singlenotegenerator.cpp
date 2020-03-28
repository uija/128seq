#include "ndlr/singlenotegenerator.h"
SingleNoteGenerator::SingleNoteGenerator() : EuclideanGenerator( MELODY_SINGLE) {
    _intervaloffset = 0;
}
SingleNoteGenerator::SingleNoteGenerator( int8_t offset) : EuclideanGenerator( MELODY_SINGLE) {
    _intervaloffset = offset;
}
Chord* SingleNoteGenerator::getRealStepChord( uint16_t step, Progression* progression) {
    uint8_t note = progression->getCurrentNote( _intervaloffset*12);
    return new Chord( new Note( note, _noteVelocity, _noteLength, _legato));
}
bool SingleNoteGenerator::isLegato() {
    return _legato;
}
void SingleNoteGenerator::setLegato( bool legato) {
    _legato = legato;
}
int8_t SingleNoteGenerator::getIntervalOffset() {
    return _intervaloffset;
}
void SingleNoteGenerator::setIntervalOffset( int8_t io) {
    _intervaloffset = io;
}
