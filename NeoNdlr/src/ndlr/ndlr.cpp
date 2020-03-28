#include "ndlr/ndlr.h"

Ndlr::Ndlr() {
    _interval = 0;
    _intervalCount = 0;
    _syncCount = 0;
    _syncInterval = 0;
    _progressionIndexQueue = 0;
    _progressionPatternIndexQueue = 0;
    _progressionPatternIndex = 0;

    this->setSyncValue( DEFAULT_SYNC);

    _generators = new Generator*[TRACKCOUNT*PATTERNCOUNT];
    _progressionPatterns = new ProgressionPattern*[PATTERNCOUNT];

    for( uint8_t t = 0; t < TRACKCOUNT; ++t) {
      _patternIndex[t] = -1;
      _patternIndexQueue[t] = -1;
      _muteQueue[t] = false;
      for( uint8_t g = 0; g < PATTERNCOUNT; ++g) {
        _generators[t*PATTERNCOUNT + g] = 0;
        if( t == 0) {
          _progressionPatterns[g] = new ProgressionPattern();
        }
      }
    }
    this->setBpm( DEFAULT_BPM);
}
Ndlr::~Ndlr() {
  for( uint8_t t = 0; t < TRACKCOUNT; ++t) {
      _patternIndex[t] = -1;
      for( uint8_t g = 0; g < PATTERNCOUNT; ++g) {
        if( _generators[t*PATTERNCOUNT + g] != 0) {
          delete _generators[t*PATTERNCOUNT + g];
          _generators[t*PATTERNCOUNT + g] = 0;
        }
      }
    }
}
void Ndlr::setBpm( uint8_t bpm) {
    _bpm = bpm;
    _interval = BPMTIME / _bpm / BPMTIMEFACTOR;  
}
void Ndlr::toggle() {
    _running = !_running;
    _syncInterval = 0;
    _syncCount = 0;
    _progressionPatterns[_progressionPatternIndex]->reset();
    if( !_running) {
      for( uint8_t i = 0; i < TRACKCOUNT; ++i) {
        _tracks[i].stop();
      }
    }
}
Generator* Ndlr::getGenerator( uint8_t trackIdx, uint8_t patternIdx) {
  if( trackIdx >= TRACKCOUNT || patternIdx >= PATTERNCOUNT) {
    return 0;
  }
  return _generators[trackIdx*PATTERNCOUNT+patternIdx];
}
bool Ndlr::setGenerator( uint8_t trackIdx, uint8_t patternIdx, Generator* generator) {
  if( trackIdx >= TRACKCOUNT || patternIdx >= PATTERNCOUNT) {
    return false;
  }
  if( _generators[trackIdx*PATTERNCOUNT+patternIdx] != 0) {
    delete _generators[trackIdx*PATTERNCOUNT+patternIdx];
  }
  _generators[trackIdx*PATTERNCOUNT+patternIdx] = generator;
}
Track* Ndlr::getTrack( uint8_t idx) {
  if( idx >= TRACKCOUNT) {
    return 0;
  }
  return &_tracks[idx];
}
bool Ndlr::onClock() {
  bool didSomething = false;
  // if there is no track active, we need to start directly
  bool isActive = false;
  for( uint8_t i = 0; i < TRACKCOUNT; ++i) {
    if( _patternIndex[i] > -1) {
      isActive = true;
    }
  }
  // handle Sync calls every sync beat
  if( !isActive || (_syncCount == 0 && _syncInterval == 0)) {
    for( uint8_t i = 0; i < TRACKCOUNT; ++i) {
      // Patterns
      if( _patternIndexQueue[i] != _patternIndex[i]) {
        _patternIndex[i] = _patternIndexQueue[i];
        // reset sync in case we are here because of isActive
        _syncInterval = 0;
        _syncCount = 0;
        didSomething = true;
      }
      // Mute
      if( _tracks[i].isMuted() != _muteQueue[i]) {
        _tracks[i].toggle();
        didSomething = true;
      }
      // Progression Steps
      if( _progression.getCurrentIndex() != _progressionIndexQueue) {
        _progression.setCurrentIndex( _progressionIndexQueue);
        didSomething = true;
      }
      // Progression Patterns
      if( _progressionPatternIndex != _progressionPatternIndexQueue) {
        _progressionPatternIndex = _progressionPatternIndexQueue;
        _progressionPatterns[_progressionPatternIndex]->reset();
        didSomething = true;
      }
    }
  }
  if( _syncInterval == 0) {
    int8_t i = _progressionPatterns[_progressionPatternIndex]->tick();
    if( i > -1) {
      _progressionIndexQueue = i;
      _progression.setCurrentIndex( i);
      didSomething = true;
    }
  }
  _syncInterval++;
  if( _syncInterval >= SYNC_DIVIDER) {
    _syncCount++;
    _syncInterval = 0;
    if( _syncCount >= _syncValue) {
      _syncCount = 0;
    }
  }
  for( uint8_t i = 0; i < TRACKCOUNT; ++i) {
    if( _patternIndex[i] > -1 && _generators[i*PATTERNCOUNT+_patternIndex[i]] != 0) {
      _tracks[i].tick( _generators[i*PATTERNCOUNT+_patternIndex[i]], &_progression);
    }
  }
  return didSomething;
}
bool Ndlr::tick() {
  if( !_running) {
    return false;
  }
  _intervalCount ++;
  bool didSomething = false;
  if( _intervalCount >= _interval) {
      didSomething = this->onClock();
      _intervalCount -= _interval;
  }
  return didSomething;
}
uint8_t Ndlr::getBpm() {
  return _bpm;
}
bool Ndlr::isRunning() {
  return _running;
}
int8_t Ndlr::getActivePattern( uint8_t trackIdx) {
  if( trackIdx >= TRACKCOUNT) return -1;
  return _patternIndex[trackIdx];
}
int8_t Ndlr::getQueuedPattern( uint8_t trackIdx) {
  if( trackIdx >= TRACKCOUNT) return -1;
  return _patternIndexQueue[trackIdx];
}
void Ndlr::setActivePattern( uint8_t trackIdx, int8_t patternIdx) {
  this->setActivePattern( trackIdx, patternIdx, false);
}
void Ndlr::setActivePattern( uint8_t trackIdx, int8_t patternIdx, bool instant) {
  if( trackIdx >= TRACKCOUNT) return;
  if( patternIdx >= PATTERNCOUNT) return;
  if( instant) {
    _patternIndex[trackIdx] = patternIdx;
    _patternIndexQueue[trackIdx] = patternIdx;
  } else {
    _patternIndexQueue[trackIdx] = patternIdx;
  }
}
void Ndlr::setSyncValue( uint8_t syncValue) {
  _syncValue = syncValue;
}
uint8_t Ndlr::getSyncValue() {
  return _syncValue;
}
uint8_t Ndlr::getSyncCount() {
  return _syncCount;
}
void Ndlr::toggle( uint8_t trackIdx, bool instant) {
  if( trackIdx >= TRACKCOUNT) return;
  if( instant) {
    _tracks[trackIdx].toggle();
    _muteQueue[trackIdx] = _tracks[trackIdx].isMuted();
  } else {
    _muteQueue[trackIdx] = !_tracks[trackIdx].isMuted();
  }
}
bool Ndlr::getMuteQueueValue( uint8_t trackIdx) {
  if( trackIdx >= TRACKCOUNT) return false;
  return _muteQueue[trackIdx];
}
Progression* Ndlr::getProgression() {
  return &_progression;
}
void Ndlr::queueNextProgressionIndex( uint8_t index) {
  _progressionIndexQueue = index;
}
ProgressionPattern* Ndlr::getProgressionPatternAt( uint8_t idx) {
  if( idx >= PATTERNCOUNT) return 0;
  return _progressionPatterns[idx];
}
uint8_t Ndlr::getProgressionPatternIndex() {
  return _progressionPatternIndex;
}
uint8_t Ndlr::getProgressionPatternQueuedIndex() {
  return _progressionPatternIndexQueue;
}
void Ndlr::queueProgressionPatternIndex( uint8_t index) {
  _progressionPatternIndexQueue = index;
}