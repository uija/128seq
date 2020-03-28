#include "screens/screenpatternsingle.h"
#include "ndlr/singlenotegenerator.h"

ScreenPatternSingle::ScreenPatternSingle( Ndlr* ndlr, uint8_t trackIdx, uint8_t patternIdx) : ScreenPatternEuclidean( ndlr, trackIdx, patternIdx) {
    usbMIDI.sendControlChange( 0, 3, _page);
}
void ScreenPatternSingle::renderInternal( Generator* gen) {
    ScreenPatternEuclidean::renderInternal( gen);
    _lcd->setCursor( 0, 0);
    _lcd->printf( "Single Note         ");

    SingleNoteGenerator* sng = (SingleNoteGenerator*)gen;

    if( _page == 2) {
        _lcd->setCursor( 0, 2);
        _lcd->printf( "Off: %2d", sng->getIntervalOffset());
        _lcd->setCursor( 10, 2);
        _lcd->printf( "Velo: %2d", sng->getNoteVelocity());
        _lcd->setCursor( 0, 3);
        _lcd->printf( "Len: %2d", sng->getNoteLength());
    }
}
uint8_t ScreenPatternSingle::getPageCount() {
    return ScreenPatternEuclidean::getPageCount() + 1;
}
UIEvent* ScreenPatternSingle::onEncoderChanged( uint8_t index, int8_t value) {
    UIEvent* e = ScreenPatternEuclidean::onEncoderChanged( index, value);
    if( e != 0) return e;
    if( _page != ScreenPatternEuclidean::getPageCount()) return 0;
    SingleNoteGenerator* sng = (SingleNoteGenerator*)_ndlr->getGenerator( _trackIdx, _patternIdx);
    if( index == 2) {
        if( value > 0 && sng->getIntervalOffset() < 64) {
            sng->setIntervalOffset( sng->getIntervalOffset() + 1);
        } else if( value < 0 && sng->getIntervalOffset() > -64) {
            sng->setIntervalOffset( sng->getIntervalOffset() - 1);
        }
    } else if( index == 0) {
        if( value > 0 && sng->getNoteVelocity() < 127) {
            sng->setNoteVelocity( sng->getNoteVelocity() + 1);
        } else if( value < 0 && sng->getNoteVelocity() > 0) {
            sng->setNoteVelocity( sng->getNoteVelocity() - 1);
        }
    } else if( index == 3) {
        if( value > 0 && sng->getNoteLength() < 127) {
            sng->setNoteLength( sng->getNoteLength() + 1);
        } else if( value < 0 && sng->getNoteLength() > 0) {
            sng->setNoteLength( sng->getNoteLength() - 1);
        }
    }
    _needsUpdate = true;
    return 0;
}