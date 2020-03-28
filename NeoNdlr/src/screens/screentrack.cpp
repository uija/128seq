#include "screens/screentrack.h"
#include "screens/screenmain.h"
#include "ui/uievent.h"

ScreenTrack::ScreenTrack( Ndlr* ndlr, uint8_t trackIdx) : UIScreen() {
    _needsUpdate = true;
    _ndlr = ndlr;
    _trackIdx = trackIdx;
}
void ScreenTrack::render() {
    if( !_needsUpdate) return;
    _lcd->setCursor( 0, 0);
    _lcd->printf( "Track %d", _trackIdx);
    _lcd->setCursor( 0, 1);
    Track* t = _ndlr->getTrack( _trackIdx);
    if( t == 0) return;
    int8_t mc = t->getMidiChannel();
    if( mc > 0) {
        _lcd->printf( "MidiChannel: %3d", mc);
    } else {
        _lcd->printf( "MidiChannel: off");
    }
    // render midi channels
    for( uint8_t i = 0; i < 16; ++i) {
        if( mc == (i+1)) {
            _trellis->setPixelColor( i, 0x00FF00);
        } else {
            _trellis->setPixelColor( i, 0x555555);
        }
    }
    _trellis->setPixelColor( 16, 0xFF0000);

    _trellis->setPixelColor( 120, 0xFF0000);

    _trellis->show();
    _needsUpdate = false;
}
UIEvent* ScreenTrack::onIndexPressed( uint8_t index) {
    UIEvent* e = 0;
    if( index == 120) {
        e = new UIEvent( new ScreenMain( _ndlr));
    }
    if( index < 16) {
        Track* track = _ndlr->getTrack( _trackIdx);
        track->setMidiChannel( index+1);
        _needsUpdate = true;
    }
    if( index == 16) {
        Track* track = _ndlr->getTrack( _trackIdx);
        track->setMidiChannel( -1);
        _needsUpdate = true;
    }
    return e;
}
UIEvent* ScreenTrack::onIndexReleased( uint8_t index) {
    UIEvent* e = 0;

    return e;
}
UIEvent* ScreenTrack::onEncoderChanged( uint8_t index, int8_t value) {
    UIEvent* e = 0;
    Track * track = _ndlr->getTrack( _trackIdx);
    if( track == 0) return e;
    int8_t mc = track->getMidiChannel();
    if( value > 0) {
        if( mc == -1) {
            track->setMidiChannel( 1);
        } else if( mc < 16) {
            track->setMidiChannel( mc+1);
        }
    } else if( value < 0) {
        if( mc == 1) {
            track->setMidiChannel( -1);
        } else if( mc > 1) {
            track->setMidiChannel( mc-1);
        }
    }
    _needsUpdate = true;
    return e;
}
void ScreenTrack::forceUpdate() {
    
}