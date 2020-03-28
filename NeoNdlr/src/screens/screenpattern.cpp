#include "screens/screenpattern.h"
#include "screens/screenmain.h"
#include "ndlr/generatorhelper.h"

ScreenPattern::ScreenPattern( Ndlr* ndlr, uint8_t trackIdx, uint8_t patternIdx) {
    _ndlr = ndlr;
    _trackIdx = trackIdx;
    _patternIdx = patternIdx;
    _page = 0;
    _needsUpdate = true;
    _selectedGenerator = 0;
    Generator* g = ndlr->getGenerator( trackIdx, patternIdx);
    if( g != 0) {
        _selectedGenerator = g->getMelody();
    }
    usbMIDI.sendControlChange( 0, 0, _page);
}
void ScreenPattern::render() {
    if( !_needsUpdate) return;

    _lcd->setCursor( 0, 0);
    _lcd->printf( "No Generator");

    // back button
    _trellis->setPixelColor( 127, 0xFF0000);

    // pages
    for( uint8_t i = 0; i < this->getPageCount() ; ++i) {
        if( i == _page) {
            _trellis->setPixelColor( 112+i, 0x00FF00);
        } else {
            _trellis->setPixelColor( 112+i, 0x333333);
        } 
    }
    Generator* gen = _ndlr->getGenerator( _trackIdx, _patternIdx);

    if( _page == 0) {
        // render basic pattern config
        _lcd->setCursor( 0, 2);
        _lcd->printf( "Select generator:");
        _lcd->setCursor( 0, 3);
        GeneratorHelper* gh = GeneratorHelper::instance();

        for( uint8_t i = 0; i < gh->getCount(); ++i) {
            if( i == _selectedGenerator) {
                const char* name = gh->getNameForIndex( i);
                if( gen != 0 && _selectedGenerator == gen->getMelody()) {
                    _lcd->printf( "!%s        ", name);
                } else {
                    _lcd->printf( "?%s        ", name);
                }
            }
        }

        if( gen == 0 || _selectedGenerator != gen->getMelody()) {
            _trellis->setPixelColor( 48, 0x00FF00);
        } else {
            _trellis->setPixelColor( 48, 0x000000);
        }
    }
    if( gen != 0) {
        this->renderInternal( gen);
    }

    _trellis->show();

    _lcd->setCursor( 13, 0);
    _lcd->printf( "T:%d P:%d", _trackIdx, _patternIdx);

    _needsUpdate = false;
}
UIEvent* ScreenPattern::onIndexPressed( uint8_t index) {
    if( index == 127) {
        return new UIEvent( new ScreenMain( _ndlr));
    }
    for( uint8_t i = 0; i < this->getPageCount(); ++i) {
        if( index == 112 + i && i != _page) {
            _page = i;
            _lcd->clear();
            for( uint8_t i = 0; i < 128; ++i) {
                _trellis->setPixelColor( i, 0x000000);
            }
            _needsUpdate = true;
        } 
    }
    if( _page == 0) {
        Generator* gen = _ndlr->getGenerator( _trackIdx, _patternIdx);
        GeneratorHelper* gh = GeneratorHelper::instance();

        if( index == 48 && (gen == 0 || _selectedGenerator != gen->getMelody())) {
            Generator* newgen = gh->getGeneratorForIndex( _selectedGenerator);
            gh->syncGenerators( gen, newgen);
            if( _ndlr->setGenerator( _trackIdx, _patternIdx, newgen)) {
                UIScreen* screen = gh->getScreenForGenerator( newgen, _ndlr, _trackIdx, _patternIdx);
                if( screen != 0) {
                    return new UIEvent( screen);
                }
                
            }
        }
    }
    return 0;
}
UIEvent* ScreenPattern::onIndexReleased( uint8_t index) {
    return 0;
}
UIEvent* ScreenPattern::onEncoderChanged( uint8_t index, int8_t value) {
    usbMIDI.sendControlChange( 0, index, 15);
    if( _page != 0) return 0;
    if( value > 0 && _selectedGenerator < GeneratorHelper::instance()->getCount() - 1) {
        _selectedGenerator++;
    } else if( value < 0 && _selectedGenerator > 0) {
        _selectedGenerator--;
    }
    _needsUpdate = true;
    return 0;
}
void ScreenPattern::forceUpdate() {
    _needsUpdate = true;
}