#include "ndlr/arpgenerator.h"

ArpGenerator::ArpGenerator() : EuclideanGenerator( MELODY_ARP) {
    _arpIndex = 0;
    _arpSize = 4;
    _normalize = false;
}

Chord* ArpGenerator::getRealStepChord( uint16_t step, Progression* progression) {
    Chord* chord = progression->getCurrentChord( 0, _noteVelocity, _noteLength, false);
    //chord->rescale( _arpSize);

    if( _normalize) {
        chord->normalize( progression->getRootNote());
    }

    // now iterate over this chord
    uint8_t chordIdx = _arpIndex;
    uint8_t offset = 0;
    while( chordIdx >= chord->getLength()) {
        chordIdx -= chord->getLength();
        offset += 12;
    }

    _arpIndex++;

    if( _arpIndex >= _arpSize) {
        _arpIndex = 0;
    }


    Note* n = chord->getNote( chordIdx);
    n->value += offset;
    Chord* ret = new Chord( n);
    delete n;
    delete chord;


    return ret;
}
void ArpGenerator::setArpSize( uint8_t size) {
    _arpSize = size;
}
uint8_t ArpGenerator::getArpSize() {
    return _arpSize;
}
void ArpGenerator::reset() {
    _arpIndex = 0;
}
bool ArpGenerator::isNormalize() {
    return _normalize;
}
void ArpGenerator::setNormalize( bool normalize) {
    _normalize = normalize;
}