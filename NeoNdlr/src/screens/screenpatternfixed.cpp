#include "screens/screenpatternfixed.h"
#include "ndlr/fixednotegenerator.h"

ScreenPatternFixed::ScreenPatternFixed( Ndlr* ndlr, uint8_t trackIdx, uint8_t patternIdx) : ScreenPatternEuclidean( ndlr, trackIdx, patternIdx) {
    usbMIDI.sendControlChange( 0, 2, _page);
}
void ScreenPatternFixed::renderInternal( Generator* gen) {
    ScreenPatternEuclidean::renderInternal( gen);
    _lcd->setCursor( 0, 0);
    _lcd->printf( "Fixed Note          ");

    FixedNoteGenerator* fng = (FixedNoteGenerator*)gen;
    if( _page != ScreenPatternEuclidean::getPageCount()) return;

    _lcd->setCursor( 0, 2);
    _lcd->printf( "Note: %2d", fng->getNoteValue());
    _lcd->setCursor( 10, 2);
    _lcd->printf( "Velo: %2d", fng->getNoteVelocity());
    _lcd->setCursor( 0, 3);
    _lcd->printf( "Len: %2d", fng->getNoteLength());
}
uint8_t ScreenPatternFixed::getPageCount() {
    return ScreenPatternEuclidean::getPageCount() + 1;
}
UIEvent* ScreenPatternFixed::onEncoderChanged( uint8_t index, int8_t value) {
    UIEvent* e = ScreenPatternEuclidean::onEncoderChanged( index, value);
    if( e != 0) return e;
    if( _page != ScreenPatternEuclidean::getPageCount()) return 0;
    FixedNoteGenerator* fng = (FixedNoteGenerator*)_ndlr->getGenerator( _trackIdx, _patternIdx);
    
    if( index == 2) {
        if( value > 0 && fng->getNoteValue() < 127) {
            fng->setNoteValue( fng->getNoteValue() + 1);
        } else if( value < 0 && fng->getNoteValue() > 0) {
            fng->setNoteValue( fng->getNoteValue() - 1);
        }
    } else if( index == 0) {
        if( value > 0 && fng->getNoteVelocity() < 127) {
            fng->setNoteVelocity( fng->getNoteVelocity() + 1);
        } else if( value < 0 && fng->getNoteVelocity() > 0) {
            fng->setNoteVelocity( fng->getNoteVelocity() - 1);
        }
    } else if( index == 3) {
        if( value > 0 && fng->getNoteLength() < 127) {
            fng->setNoteLength( fng->getNoteLength() + 1);
        } else if( value < 0 && fng->getNoteLength() > 0) {
            fng->setNoteLength( fng->getNoteLength() - 1);
        }
    }
    _needsUpdate = true;

    return 0;
}