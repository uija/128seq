#include "ndlr/fixednotegenerator.h"

FixedNoteGenerator::FixedNoteGenerator() : EuclideanGenerator( MELODY_FIXED) {
    _noteValue = 64;
}
FixedNoteGenerator::FixedNoteGenerator( uint8_t value) : EuclideanGenerator( MELODY_FIXED) {
    _noteValue = value;
}
Chord* FixedNoteGenerator::getRealStepChord( uint16_t step, Progression* progression) {
    return new Chord( new Note( _noteValue, _noteVelocity, _noteLength, false));
}
void FixedNoteGenerator::setNoteValue( uint8_t value) {
    _noteValue = value;
}
uint8_t FixedNoteGenerator::getNoteValue() {
    return _noteValue;
}