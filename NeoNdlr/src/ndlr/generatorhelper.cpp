#include "ndlr/generatorhelper.h"

#include "screens/screenpatternfixed.h"
#include "screens/screenpatternsingle.h"
#include "screens/screenpatternarp.h"
#include "screens/screenpatternpad.h"

#include "ndlr/fixednotegenerator.h"
#include "ndlr/singlenotegenerator.h"
#include "ndlr/arpgenerator.h"
#include "ndlr/padgenerator.h"

GeneratorHelper* GeneratorHelper::_instance = 0;

GeneratorHelper* GeneratorHelper::instance() {
    if( _instance == 0) {
        _instance = new GeneratorHelper();
    }
    return _instance;
}

const char* GeneratorHelper::getNameForIndex( uint8_t idx) {
    switch( idx) {
        case MELODY_FIXED: return "Fixed Note";
        case MELODY_SINGLE: return "Single Note";
        case MELODY_ARP: return "Arpeggio";
        case MELODY_PAD: return "Pad      ";
    }
    return "";
}
uint8_t GeneratorHelper::getCount() {
    return MELODY_COUNT;
}
Generator* GeneratorHelper::getGeneratorForIndex( uint8_t idx) {
    switch( idx) {
        case MELODY_FIXED: return new FixedNoteGenerator();
        case MELODY_SINGLE: return new SingleNoteGenerator();
        case MELODY_ARP: return new ArpGenerator();
        case MELODY_PAD: return new PadGenerator();
    }
    return 0;
}
UIScreen* GeneratorHelper::getScreenForGenerator( Generator* gen, Ndlr* ndrl, uint8_t trackIdx, uint8_t patternIdx) {
    if( gen != 0) {
        switch( gen->getMelody()) {
            case MELODY_FIXED: return new ScreenPatternFixed( ndrl, trackIdx, patternIdx);
            case MELODY_SINGLE: return new ScreenPatternSingle( ndrl, trackIdx, patternIdx);
            case MELODY_ARP: return new ScreenPatternArp( ndrl, trackIdx, patternIdx);
            case MELODY_PAD: return new ScreenPatternPad( ndrl, trackIdx, patternIdx);
        }
    }
    return new ScreenPattern( ndrl, trackIdx, patternIdx);
}
void GeneratorHelper::syncGenerators( Generator* source, Generator* target) {
    if( source == 0 || target == 0) return;
    if( source->getType() == TYPE_EUCLIDEAN && target->getType() == TYPE_EUCLIDEAN) {
        EuclideanGenerator* from = (EuclideanGenerator*)source;
        EuclideanGenerator* to = (EuclideanGenerator*)target;
        to->initialize( from->getPatternLength(), from->getPulses(), from->getOffset());
    }
}