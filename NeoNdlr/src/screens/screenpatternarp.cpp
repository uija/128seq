#include "screens/screenpatternarp.h"
#include "ndlr/arpgenerator.h"

ScreenPatternArp::ScreenPatternArp( Ndlr* ndlr, uint8_t trackIdx, uint8_t patternIdx) : ScreenPatternEuclidean( ndlr, trackIdx, patternIdx) {
}
void ScreenPatternArp::renderInternal( Generator* gen) {
    ScreenPatternEuclidean::renderInternal( gen);
    _lcd->setCursor( 0, 0);
    _lcd->printf( "Arpeggio      ");

    ArpGenerator* ag = (ArpGenerator*)gen;

    if( _page == 2) {
        _lcd->setCursor( 0, 2);
        _lcd->printf( "Norm: %d", ag->isNormalize());
        _lcd->setCursor( 10, 2);
        _lcd->printf( "Velo: %2d", ag->getNoteVelocity());
        _lcd->setCursor( 0, 3);
        _lcd->printf( "Len: %2d", ag->getNoteLength());
        _lcd->setCursor( 10, 3);
        _lcd->printf( "Size: %2d", ag->getArpSize());
    }
}
uint8_t ScreenPatternArp::getPageCount() {
    return ScreenPatternEuclidean::getPageCount() + 1;
}
UIEvent* ScreenPatternArp::onEncoderChanged( uint8_t index, int8_t value) {
    UIEvent* e = ScreenPatternEuclidean::onEncoderChanged( index, value);
    if( e != 0) return e;
    if( _page != ScreenPatternEuclidean::getPageCount()) return 0;
    ArpGenerator* ag = (ArpGenerator*)_ndlr->getGenerator( _trackIdx, _patternIdx);
    if( index == 0) {
        if( value > 0 && ag->getNoteVelocity() < 127) {
            ag->setNoteVelocity( ag->getNoteVelocity() + 1);
        } else if( value < 0 && ag->getNoteVelocity() > 0) {
            ag->setNoteVelocity( ag->getNoteVelocity() - 1);
        }
    } else if( index == 1) {
        if( value > 0 && ag->getArpSize() < 8) {
            ag->setArpSize( ag->getArpSize() + 1);
        } else if( value < 0 && ag->getArpSize() > 0) {
            ag->setArpSize( ag->getArpSize() - 1);
        }
    } else if( index == 2) {
        if( value != 0) {
            ag->setNormalize( !ag->isNormalize());
        }
    } else if( index == 3) {
        if( value > 0 && ag->getNoteLength() < 127) {
            ag->setNoteLength( ag->getNoteLength() + 1);
        } else if( value < 0 && ag->getNoteLength() > 0) {
            ag->setNoteLength( ag->getNoteLength() - 1);
        }
    }
    _needsUpdate = true;
    return 0;
}