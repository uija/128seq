#include "screens/screenprogressionpattern.h"

ScreenProgressionPattern::ScreenProgressionPattern( Ndlr* ndlr, uint8_t patternIdx) {
    _ndlr = ndlr;
    _patternIdx = patternIdx;
    _needsUpdate = true;
    _currentStep = 0;
}
void ScreenProgressionPattern::render() {
    if( !_needsUpdate) return;
    _lcd->setCursor( 0, 0);
    _lcd->printf( "ProgPat: %d", _patternIdx);


    ProgressionPattern* pp = _ndlr->getProgressionPatternAt( _patternIdx);
    if( pp == 0) {
        return;
    }

    _lcd->setCursor( 0, 2);
    _lcd->printf( "Step: %2d", _currentStep);
    _lcd->setCursor( 0, 3);
    _lcd->printf( "Int: %d", pp->getIntervalAt( _currentStep));
    _lcd->setCursor( 10, 3);
    _lcd->printf( "Dur: %2d", pp->getDurationAt( _currentStep));

    for( uint8_t i = 0; i < PROGRESSION_STEP_LENGTH; ++i) {
        if( _currentStep == i) {
            _trellis->setPixelColor( i, 0xFF8000);
        } else if( pp->getDurationAt( i) > 0) {
            _trellis->setPixelColor( i, 0xFFFF00);
        } else {
            _trellis->setPixelColor( i, 0x333333);
        } 
    }

    // render interval
    for( uint8_t i = 0; i < 8; ++i) {
        if( pp->getIntervalAt( _currentStep) == i) {
            _trellis->setPixelColor( 2*16+i, 0xFF8000);
        } else {
            _trellis->setPixelColor( 2*16+i, 0x553300);
        }
    }
    // render duration
    for( uint8_t i = 0; i < 64; ++i) {
        if( pp->getDurationAt( _currentStep) > i) {
            _trellis->setPixelColor( 3*16+i, 0x0000FF);
        } else {
            _trellis->setPixelColor( 3*16+i, 0x333355);
        }
    }

    // backbutton
    _trellis->setPixelColor( 127, 0xFF0000);
    _trellis->show();

    _needsUpdate = false;
}
UIEvent* ScreenProgressionPattern::onIndexPressed( uint8_t index) {
    UIEvent* event = 0;
    if( index == 127) {
        event = new UIEvent( new ScreenMain( _ndlr));
    }
    // step
    if( index < PROGRESSION_STEP_LENGTH) {
        _currentStep = index;
        _needsUpdate = true;
        return event;
    }
    // interval
    ProgressionPattern* pp = _ndlr->getProgressionPatternAt( _patternIdx);
    if( index >= 2*16 && index < 2*16+8) {
        uint8_t i = index - 2*16;
        pp->setIntervalAt( _currentStep, i);
        _needsUpdate = true;
        return event;
    }
    if( index >= 3*16 && index < 7*16) {
        uint8_t i = index - 3*16 + 1;
        if( i == pp->getDurationAt( _currentStep)) {
            pp->setDurationAt( _currentStep, 0);
        } else {
            pp->setDurationAt( _currentStep, i);
        }
        _needsUpdate = true;
    }
    return event;
}
UIEvent* ScreenProgressionPattern::onIndexReleased( uint8_t index) {
    return 0;
}
UIEvent* ScreenProgressionPattern::onEncoderChanged( uint8_t index, int8_t value) {
    UIEvent* event = 0;

    ProgressionPattern* pp = _ndlr->getProgressionPatternAt( _patternIdx);
    if( pp == 0) {
        return 0;
    }

    if( index == 2) {
        if( value > 0 && _currentStep < PROGRESSION_STEP_LENGTH-1) {
            _currentStep++;
        } else if( value < 0 && _currentStep > 0) {
            _currentStep--;
        }
    } else if( index == 1) {
        if( value > 0 && pp->getDurationAt( _currentStep) < 63) {
            pp->setDurationAt( _currentStep, pp->getDurationAt( _currentStep)+1);
        } else if( value < 0 && pp->getDurationAt( _currentStep) > 0) {
            pp->setDurationAt( _currentStep, pp->getDurationAt( _currentStep)-1);
        }
    } else if( index == 3) {
        if( value > 0 && pp->getIntervalAt( _currentStep) < 7) {
            pp->setIntervalAt( _currentStep, pp->getIntervalAt( _currentStep)+1);
        } else if( value < 0 && pp->getIntervalAt( _currentStep) > 0) {
            pp->setIntervalAt( _currentStep, pp->getIntervalAt( _currentStep)-1);
        }
    }
    _needsUpdate = true;

    return event;
}
void ScreenProgressionPattern::forceUpdate() {
    _needsUpdate = true;
}