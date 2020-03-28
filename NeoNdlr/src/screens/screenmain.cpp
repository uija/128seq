#include "screens/screenmain.h"
#include "screens/screentrack.h"
#include "ndlr/generatorhelper.h"
#include "screens/screenprogressionpattern.h"
#include "ui/uievent.h"

ScreenMain::ScreenMain( Ndlr* ndlr) : UIScreen() {
    _needsUpdate = true;
    _shift = false;
    _edit = false;
    _ndlr = ndlr;
}
void ScreenMain::render() {
    if( _needsUpdate) {
        _lcd->setCursor( 0, 2);
        _lcd->printf( "%3dbpm", _ndlr->getBpm());
        _lcd->setCursor( 10, 2);
        _lcd->printf( "Sync:%2d/16", _ndlr->getSyncValue());
        // tracks
        for( uint8_t i = 0; i < TRACKCOUNT; ++i) {
            Track* t = _ndlr->getTrack( i);
            if( t != 0 && t->isActive()) {
                if( t->isMuted() && _ndlr->getMuteQueueValue( i)) {
                    _trellis->setPixelColor( 112+i, 0xFF0000);
                } else if( t->isMuted() && !_ndlr->getMuteQueueValue( i)) {
                    _trellis->setPixelColor( 112+i, 0x88FF88);
                } else if( !t->isMuted() && _ndlr->getMuteQueueValue( i)) {
                    _trellis->setPixelColor( 112+i, 0xFF8888);
                } else {
                    _trellis->setPixelColor( 112+i, 0x0000FF);
                }
            } else {
                _trellis->setPixelColor( 112+i, 0x3333388);
            }
        }
        // render patterns
        for( uint8_t t = 0; t < TRACKCOUNT; ++t) {
            int8_t ap = _ndlr->getActivePattern( t);
            int8_t qp = _ndlr->getQueuedPattern( t);
            for( uint8_t p = 0; p < PATTERNCOUNT; ++p) {
                if( p == ap && (qp == ap || qp != -1)) {
                    _trellis->setPixelColor( t+p*16, 0x00FF00);
                } else if( p == qp) {
                    _trellis->setPixelColor( t+p*16, 0x5500FF);
                } else if( ap == p && qp == -1) {
                    _trellis->setPixelColor( t+p*16, 0xFF0055);
                } else if( _ndlr->getGenerator( t, p) != 0) {
                    _trellis->setPixelColor( t+p*16, 0x88FF88);
                } else {
                    _trellis->setPixelColor( t+p*16, 0x555555);
                }
            }
        }
        // render ProgressionSteps
        for( uint8_t i = 0; i < PATTERNCOUNT; ++i) {
            ProgressionPattern* pp = _ndlr->getProgressionPatternAt( i);
            if( pp != 0 && pp->isActive()) {
                if( i == _ndlr->getProgressionPatternIndex()) {
                    _trellis->setPixelColor( i*16+9, 0x0000FF);
                } else if( i == _ndlr->getProgressionPatternQueuedIndex()) {
                    _trellis->setPixelColor( i*16+9, 0xFF00FF);
                } else {
                    _trellis->setPixelColor( i*16+9, 0x5555FF);
                }
            } else {
                _trellis->setPixelColor( i*16+9, 0x333333);
            }
        }
        // render Progression
        Progression* p = _ndlr->getProgression();
        if( p != 0) {
            for( uint8_t i = 0; i < 8; ++i) {
                if( p->getCurrentIndex() == i) {
                    _trellis->setPixelColor( i*16+13, 0x00FF00);
                } else {
                    _trellis->setPixelColor( i*16+13, 0x338833);
                }
            }
        }
        // play button
        if( _ndlr->isRunning()) {
            _trellis->setPixelColor( 15, 0xFF0000);
        } else {
            _trellis->setPixelColor( 15, 0x00FF00);
        }
        
        // shift
        _trellis->setPixelColor( 127, 0x888888);
        // track edit
        _trellis->setPixelColor( 120, 0x888888);
        _trellis->show();
    }
   
    _needsUpdate = false;
}
UIEvent* ScreenMain::onIndexPressed( uint8_t index) {
    UIEvent* event = 0;
    if( index == 15) {
        _ndlr->toggle();
        if( _ndlr->isRunning()) {
            _trellis->setPixelColor( 15, 0xFF0000);
        } else {
            _trellis->setPixelColor( 15, 0x00FF00);
        }
        _needsUpdate = true;
    }
    // shift
    if( index == 127) {
        _shift = true;
        _trellis->setPixelColor( 127, 0xFFFFFF);
        _trellis->show();
    }
    if( index == 120) {
        _edit = true;
        _trellis->setPixelColor( 120, 0xFFFFFF);
        _trellis->show();
    }
    // patterns
    for( uint8_t t = 0; t < TRACKCOUNT; ++t) {
        for( uint8_t p = 0; p < PATTERNCOUNT; ++p) {
            uint8_t idx = p*16+t;
            if( idx == index) {
                if( !_shift) {
                    if( _ndlr->getActivePattern( t) == p) {
                        _ndlr->setActivePattern( t, -1);
                    } else if( _ndlr->getGenerator( t, p) != 0) {
                        _ndlr->setActivePattern( t, p);
                    }              
                    _needsUpdate = true;
                } else {
                    // handle all the different patterns
                    Generator* g = _ndlr->getGenerator( t, p);
                    UIScreen* screen = GeneratorHelper::instance()->getScreenForGenerator( g, _ndlr, t, p);
                    if( screen != 0) {
                        return new UIEvent( screen);
                    }
                }
            }
        }
    }
    // tracks
    for( uint8_t t = 0; t < TRACKCOUNT; ++t) {
        if( index == 112+t) {
            if( _edit) {
                event = new UIEvent( new ScreenTrack( _ndlr, t));
            } else if( _shift || !_ndlr->isRunning()) {
                _ndlr->toggle( t, true);
            } else {
                _ndlr->toggle( t, false);
            }
            _needsUpdate = true;            
        }
    }
    // progression
    Progression* p = _ndlr->getProgression();
    for( uint8_t i = 0; i < 8; ++i) {
        if( p->getCurrentIndex() != i && index == (i*16+13)) {
            _ndlr->queueNextProgressionIndex( i);
        }
    }
    // Progression Pattern
    // render ProgressionSteps
    for( uint8_t i = 0; i < PATTERNCOUNT; ++i) {
        if( index == i*16+9) {
            ProgressionPattern* pp = _ndlr->getProgressionPatternAt( i);
            if( pp != 0) {
                if( _shift) {
                    event = new UIEvent( new ScreenProgressionPattern( _ndlr, i));
                } else if( i != _ndlr->getProgressionPatternIndex()) {
                    if( pp != 0 && pp->isActive()) {
                        _ndlr->queueProgressionPatternIndex( i);
                        _needsUpdate = true;
                    }
                }
            }
        }
        
    }
    return event;
}
UIEvent* ScreenMain::onIndexReleased( uint8_t index) {
    if( index == 127) {
        _shift = false;
        _trellis->setPixelColor( 127, 0x888888);
        _trellis->show();
    }
    if( index == 120) {
        _edit = false;
        _trellis->setPixelColor( 120, 0x888888);
        _trellis->show();
    }
    return 0;
}
UIEvent* ScreenMain::onEncoderChanged( uint8_t index, int8_t value) {
    if( index == 2) {
        short bpm = _ndlr->getBpm();
        bpm += value;
        if( bpm > 250) {
            bpm = 250;
        }
        if( bpm < 1) {
            bpm = 1;
        }
        _ndlr->setBpm( bpm);
        _needsUpdate = true;
    } else if( index == 0 && !_ndlr->isRunning()) {
        if( value > 0 && _ndlr->getSyncValue() < 126) {
            _ndlr->setSyncValue( _ndlr->getSyncValue()+1);
        } else if( value < 0 && _ndlr->getSyncValue() > 1) {
            _ndlr->setSyncValue( _ndlr->getSyncValue()-1);
        }
        _needsUpdate = true;
    }
    return 0;
}
void ScreenMain::forceUpdate() {
    _needsUpdate = true;
}