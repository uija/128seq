#include "screens/screenpatterneuclidean.h"
#include "ndlr/euclideangenerator.h"

ScreenPatternEuclidean::ScreenPatternEuclidean( Ndlr* ndlr, uint8_t trackIdx, uint8_t patternIdx) : ScreenPattern( ndlr, trackIdx, patternIdx) {
    _page = 1;
    usbMIDI.sendControlChange( 0, 1, _page);
}
UIEvent* ScreenPatternEuclidean::onIndexPressed( uint8_t index) {
    UIEvent* e = ScreenPattern::onIndexPressed( index);
    if( e != 0) return e;
    // TODO build Euclidean Buttons
}
UIEvent* ScreenPatternEuclidean::onEncoderChanged( uint8_t index, int8_t value) {
    UIEvent* e = ScreenPattern::onEncoderChanged( index, value);
    if( e != 0) return e;
    if( _page != ScreenPattern::getPageCount()) return 0;
    usbMIDI.sendControlChange( 1, index, 15);
    EuclideanGenerator* eg = (EuclideanGenerator*)_ndlr->getGenerator( _trackIdx, _patternIdx);
    if( index == 2) {
        if( value > 0 && eg->getPatternLength() < EUCLIDEAN_MAX_STEPS) {
            eg->initialize( eg->getPatternLength()+1, eg->getPulses(), eg->getOffset());
        } else if( value < 0 && eg->getPatternLength() > 1) {
            eg->initialize( eg->getPatternLength()-1, eg->getPulses(), eg->getOffset());
        }
        _needsUpdate = true;
    } else if( index == 0) {
        if( value > 0 && eg->getPulses() < eg->getPatternLength()) {
            eg->initialize( eg->getPatternLength(), eg->getPulses()+1, eg->getOffset());
        } else if( value < 0 && eg->getPulses() > 1) {
            eg->initialize( eg->getPatternLength(), eg->getPulses()-1, eg->getOffset());
        }
        _needsUpdate = true;
    } else if( index == 3) {
        if( value > 0 && eg->getOffset() < eg->getPatternLength()-1) {
            eg->initialize( eg->getPatternLength(), eg->getPulses(), eg->getOffset()+1);
        } else if( value < 0 && eg->getOffset() > 0) {
            eg->initialize( eg->getPatternLength(), eg->getPulses(), eg->getOffset()-1);
        }
        _needsUpdate = true;
    }
    return 0;
}
void ScreenPatternEuclidean::renderInternal( Generator* gen) {
    EuclideanGenerator* eg = (EuclideanGenerator*)gen;
    if( _page != ScreenPattern::getPageCount()) return;
    // Euclidean Display
    _lcd->setCursor( 0, 2);
    _lcd->printf( "Len: %2d", eg->getPatternLength());
    _lcd->setCursor( 10, 2);
    _lcd->printf( "Pul: %2d", eg->getPulses());
    _lcd->setCursor( 0, 3);
    _lcd->printf( "Off: %2d", eg->getOffset());

    // Render Euclidean Button Representation
    for( uint8_t i = 0; i < EUCLIDEAN_MAX_STEPS; ++i) {
        if( eg->getPatternLength() <= i) {
            _trellis->setPixelColor( i, 0x333333);
        } else if( eg->hasPulse( i)) {
            _trellis->setPixelColor( i, 0x00FF00);
        } else {
            _trellis->setPixelColor( i, 0xFFFFFF);
        }
    }
}
uint8_t ScreenPatternEuclidean::getPageCount() { 
    return ScreenPattern::getPageCount() + 1; 
}
