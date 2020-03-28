#include "ndlr/track.h"

Track::Track() {
    _step = 0;
    _midiChannel = -1;
    _muted = false;
    for( uint8_t i = 0; i < POLY_COUNT; ++i) {
        _stack[i].value = -1;
        _stack[i].length = -1;
    }
}

void Track::noteOn( uint8_t value, uint8_t velocity) {
    if( _midiChannel < 0 || _muted) return;
    usbMIDI.sendNoteOn( value, velocity, _midiChannel);
}
void Track::noteOff( uint8_t value, uint8_t velocity) {
    if( _midiChannel < 0) return;
    usbMIDI.sendNoteOff( value, velocity, _midiChannel);
}
void Track::playNote( Note* note) {
    // we need to evaluate an empty spot
    // First, check if the note is already playing
    uint8_t idx = POLY_COUNT;
    uint8_t emptyIdx = POLY_COUNT;
    for( uint8_t i = 0; i < POLY_COUNT; ++i) {
        if( _stack[i].value == note->value) {
            if( note->legato) {
                _stack[i].length += note->length;
                return;
            } else {
                this->noteOff( _stack[i].value, _stack[i].velocity);
                idx = i;
                _stack[i].length = -1;
                _stack[i].value = -1;
                break;
            }
        } else if( _stack[i].value == -1) {
            emptyIdx = i;
        }
    }
    if( idx == POLY_COUNT) {
        if( emptyIdx == POLY_COUNT) {
            uint8_t longestLifeTime = 0;
            for( uint8_t i = 0; i < POLY_COUNT; ++i) {
                if( _stack[i].lifetime > longestLifeTime) {
                    idx = i;
                    longestLifeTime = _stack[i].lifetime;
                }
            }
        } else {
            idx = emptyIdx;
        }
    }
    if( idx == POLY_COUNT) return;
    _stack[idx].value = note->value;
    _stack[idx].length = note->length;
    _stack[idx].velocity = note->velocity;
    _stack[idx].lifetime = 0;
    this->noteOn( note->value, note->velocity);
}

void Track::tick( Generator* generator, Progression* progression) {
    if( _midiChannel < 0) {
        return;
    }
    if( generator == 0) {
        return;
    }
    if( generator->needsReset()) {
        _step = 0;
    }

    // handle currently playing notes
    for( uint8_t i = 0; i < POLY_COUNT; ++i) {
        if( _stack[i].value >= 0) {
            _stack[i].length--;
            _stack[i].lifetime++;
            if( _stack[i].length < 0) {
                this->noteOff( _stack[i].value, _stack[i].velocity);
                _stack[i].value = -1;
                _stack[i].length = -1;
            }
        }
    }

    Chord* chord = generator->getChordForStep( _step, progression);
    if( chord != 0) {
        for( uint8_t i = 0; i < chord->getLength(); ++i) {
            Note* note = chord->getNote( i);
            this->playNote( note);
            delete note;
        }

        delete chord;
    }

    _step++;
    if( _step >= generator->getLength()) {
        _step = 0;
    }
}
void Track::setMidiChannel( int8_t midiChannel) {
    if( midiChannel != _midiChannel && _midiChannel > -1) {
        this->haltAllNotes();
    }
    _midiChannel = midiChannel;
}
int8_t Track::getMidiChannel() {
    return _midiChannel;
}
void Track::haltAllNotes() {
    for( uint8_t i = 0; i < POLY_COUNT; ++i) {
        if( _stack[i].value != -1) {
            this->noteOff( _stack[i].value, _stack[i].velocity);
            _stack[i].value = -1;
            _stack[i].length = -1;
        }
    }
}
void Track::stop() {
    this->haltAllNotes();
    _step = 0;
}
bool Track::isActive() {
    return _midiChannel >= 0;
}
void Track::toggle() {
    _muted = !_muted;
}
bool Track::isMuted() {
    return _muted;
}