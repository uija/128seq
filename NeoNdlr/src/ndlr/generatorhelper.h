#ifndef _GENERATORHELPER_H
#define _GENERATORHELPER_H

#include <Arduino.h>
#include "ndlr/ndlr.h"
#include "ui/uiscreen.h"
#include "ndlr/generator.h"

class GeneratorHelper {
private:
    static GeneratorHelper* _instance;
    GeneratorHelper() {}
    GeneratorHelper( const GeneratorHelper&) {}
public:
    static GeneratorHelper* instance();

    const char* getNameForIndex( uint8_t idx);
    uint8_t getCount();
    Generator* getGeneratorForIndex( uint8_t idx);
    UIScreen* getScreenForGenerator( Generator* gen, Ndlr* ndrl, uint8_t trackIdx, uint8_t patternIdx);
    void syncGenerators( Generator* source, Generator* target);
};
#endif