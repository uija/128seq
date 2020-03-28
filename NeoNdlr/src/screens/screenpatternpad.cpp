#include "screens/screenpatternpad.h"
#include "ndlr/padgenerator.h"

ScreenPatternPad::ScreenPatternPad( Ndlr* ndlr, uint8_t trackIdx, uint8_t patternIdx) : ScreenPatternEuclidean( ndlr, trackIdx, patternIdx) {
}
void ScreenPatternPad::renderInternal( Generator* gen) {
    ScreenPatternEuclidean::renderInternal( gen);
    _lcd->setCursor( 0, 0);
    _lcd->printf( "Pad             ");

    PadGenerator* pg = (PadGenerator*)gen;

    if( _page == 2) {
        _lcd->setCursor( 0, 2);
        _lcd->printf( "Norm: %d", pg->isNormalize());
        _lcd->setCursor( 10, 2);
        _lcd->printf( "Velo: %2d", pg->getNoteVelocity());
        _lcd->setCursor( 0, 3);
        _lcd->printf( "Len: %2d", pg->getNoteLength());
    }
}
uint8_t ScreenPatternPad::getPageCount() {
    return ScreenPatternEuclidean::getPageCount() + 1;
}
UIEvent* ScreenPatternPad::onEncoderChanged( uint8_t index, int8_t value) {
    UIEvent* e = ScreenPatternEuclidean::onEncoderChanged( index, value);
    if( e != 0) return e;
    if( _page != ScreenPatternEuclidean::getPageCount()) return 0;
    PadGenerator* pg = (PadGenerator*)_ndlr->getGenerator( _trackIdx, _patternIdx);
    if( index == 0) {
        if( value > 0 && pg->getNoteVelocity() < 127) {
            pg->setNoteVelocity( pg->getNoteVelocity() + 1);
        } else if( value < 0 && pg->getNoteVelocity() > 0) {
            pg->setNoteVelocity( pg->getNoteVelocity() - 1);
        }
    } else if( index == 2) {
        if( value != 0) {
            pg->setNormalize( !pg->isNormalize());
        }
    } else if( index == 3) {
        if( value > 0 && pg->getNoteLength() < 127) {
            pg->setNoteLength( pg->getNoteLength() + 1);
        } else if( value < 0 && pg->getNoteLength() > 0) {
            pg->setNoteLength( pg->getNoteLength() - 1);
        }
    }
    _needsUpdate = true;
    return 0;
}