#include "ndlr/padgenerator.h"

PadGenerator::PadGenerator() : EuclideanGenerator( MELODY_PAD) {
    _normalize = false;
}
Chord* PadGenerator::getRealStepChord( uint16_t step, Progression* progression) {
    Chord* chord = progression->getCurrentChord( 0, _noteVelocity, _noteLength, false);
    //chord->rescale( _arpSize);

    if( _normalize) {
        chord->normalize( progression->getRootNote());
    }

    return chord;
}
bool PadGenerator::isNormalize() {
    return _normalize;
}
void PadGenerator::setNormalize( bool normalize) {
    _normalize = normalize;
}