#ifndef _GENERATORHELPER_H
#define _GENERATORHELPER_H

#include <Arduino.h>
#include "screens/screenpatternfixed.h"
#include "screens/screenpatternsingle.h"
#include "ndlr/fixednotegenerator.h"
#include "ndlr/singlenotegenerator.h"

class GeneratorHelper {
private:
    static GeneratorHelper* _instance;
    GeneratorHelper() {}
    GeneratorHelper( const GeneratorHelper&) {}
public:
    static GeneratorHelper* instance() {
        if( _instance == 0) {
            _instance = new GeneratorHelper();
        }
        return _instance;
    }

    const char* getNameForIndex( uint8_t idx) {
        switch( idx) {
            case MELODY_FIXED: return "Fixed Note";
            case MELODY_SINGLE: return "Single Note";
        }
        return "";
    }
    uint8_t getCount() {
        return 2;
    }
    Generator* getGeneratorForIndex( uint8_t idx) {
        switch( idx) {
            case MELODY_FIXED: return new FixedNoteGenerator();
            case MELODY_SINGLE: return new SingleNoteGenerator();
        }
        return 0;
    }
    UIScreen* getScreenForIndex( uint8_t idx, Ndlr* ndrl, uint8_t trackIdx, uint8_t patternIdx) {
        switch( idx) {
            case MELODY_FIXED: return new ScreenPatternFixed( ndrl, trackIdx, patternIdx);
            case MELODY_SINGLE: return new ScreenPatternSingle( ndrl, trackIdx, patternIdx);
        }
        return 0;
    }
    UIScreen* getScreenForGenerator( Generator* gen, Ndlr* ndrl, uint8_t trackIdx, uint8_t patternIdx) {
        if( gen != 0) {
            switch( gen->getMelody()) {
                case MELODY_FIXED: return new ScreenPatternFixed( ndrl, trackIdx, patternIdx);
                case MELODY_SINGLE: return new ScreenPatternSingle( ndrl, trackIdx, patternIdx);
            }
        }
        return new ScreenPattern( ndrl, trackIdx, patternIdx);
    }
    void syncGenerators( Generator* source, Generator* target) {
        if( source == 0 || target == 0) return;
        if( source->getType() == TYPE_EUCLIDEAN && target->getType() == TYPE_EUCLIDEAN) {
            EuclideanGenerator* from = (EuclideanGenerator*)source;
            EuclideanGenerator* to = (EuclideanGenerator*)target;
            to->initialize( from->getPatternLength(), from->getPulses(), from->getOffset());
        }
    }
};
#endif