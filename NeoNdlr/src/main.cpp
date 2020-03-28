#include <Arduino.h>
#include "LiquidCrystal_I2C.h"
#include "Adafruit_NeoTrellis.h"
#include "rotencoder.h"
#include "ui/ui.h"
#include "ndlr/ndlr.h"
#include "ndlr/fixednotegenerator.h"
#include "ndlr/singlenotegenerator.h"
#include "screens/screenmain.h"

LiquidCrystal_I2C lcd( 0x27, 20, 4);
Adafruit_NeoTrellis list[2][4] = {
  { Adafruit_NeoTrellis(0x2E), Adafruit_NeoTrellis(0x2F), Adafruit_NeoTrellis(0x30), Adafruit_NeoTrellis(0x32)},
  { Adafruit_NeoTrellis(0x36), Adafruit_NeoTrellis(0x3E), Adafruit_NeoTrellis(0x31), Adafruit_NeoTrellis(0x33)},
};
Adafruit_MultiTrellis trellis( (Adafruit_NeoTrellis*)list, 2, 4);

IntervalTimer timer;

RotEncoder enc0( 0, 1);
RotEncoder enc1( 3, 4);
RotEncoder enc2( 6, 7);
RotEncoder enc3( 9, 10);

short e;

UI ui( &trellis, &lcd);
Ndlr ndlr;

TrellisCallback onKeyCallback( keyEvent event) {
  if( event.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    ui.buttonDown( event.bit.NUM);
  } else if( event.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    ui.buttonUp( event.bit.NUM);
  }
  return 0;
}

void onTimer() {
  if( ndlr.tick()) {
    ui.forceUpdate();
  }
}

void setup() {
  // Init LCD
  lcd.init();
  lcd.backlight();

  // Setup Trellis
  trellis.begin();
  // Set Brightness for every pixel
  for( uint8_t y = 0; y < 2; ++y) {
    for( uint8_t x = 0; x < 4; ++x) {
      list[y][x].pixels.setBrightness( 50);
    }
  }
  // register Key Callback
  for( uint16_t i = 0; i < 128; ++i) {
    trellis.activateKey( i, SEESAW_KEYPAD_EDGE_FALLING, true);
    trellis.activateKey( i, SEESAW_KEYPAD_EDGE_RISING, true);
    trellis.registerCallback( i, onKeyCallback);
  }

  
  // kick
  Track* t = ndlr.getTrack( 0);
  t->setMidiChannel( 1);
  FixedNoteGenerator* fng = new FixedNoteGenerator( 64);
  fng->initialize( 16, 4, 0);
  //fng->initialize( 16, 1, 0);
  ndlr.setGenerator( 0, 0, fng);

  // snare
  t = ndlr.getTrack( 1);
  t->setMidiChannel( 2);
  fng = new FixedNoteGenerator( 64);
  fng->initialize( 16, 2, 4);
  //fng->initialize( 16, 1, 0);
  ndlr.setGenerator( 1, 0, fng);

  // hihat
  t = ndlr.getTrack( 2);
  t->setMidiChannel( 3);
  fng = new FixedNoteGenerator( 64);
  fng->initialize( 16, 4, 2);
  //fng->initialize( 16, 1, 0);
  ndlr.setGenerator( 2, 0, fng);

  // bass
  t = ndlr.getTrack( 3);
  t->setMidiChannel( 4);
  SingleNoteGenerator* sng = new SingleNoteGenerator( -1);
  sng->initialize( 16, 7, 0);
  //sng->initialize( 16, 1, 0);
  ndlr.setGenerator( 3, 0, sng);

  ndlr.getProgressionPatternAt( 0)->setIntervalAt( 0, 0);
  ndlr.getProgressionPatternAt( 0)->setDurationAt( 0, 16);
  ndlr.getProgressionPatternAt( 0)->setIntervalAt( 1, 5);
  ndlr.getProgressionPatternAt( 0)->setDurationAt( 1, 16);
  ndlr.getProgressionPatternAt( 0)->setIntervalAt( 2, 4);
  ndlr.getProgressionPatternAt( 0)->setDurationAt( 2, 16);
  ndlr.getProgressionPatternAt( 0)->setIntervalAt( 3, 7);
  ndlr.getProgressionPatternAt( 0)->setDurationAt( 3, 16);

  ui.setScreen( new ScreenMain( &ndlr));
  // Start timer
  timer.begin( onTimer, 10);
}

void loop() {
    trellis.read();

    // use enc0 for track 1 length
    e = enc0.tick(); if( e != 0) { ui.updateEncoder( 0, e); }
    e = enc1.tick(); if( e != 0) { ui.updateEncoder( 1, e); }
    e = enc2.tick(); if( e != 0) { ui.updateEncoder( 2, e); }
    e = enc3.tick(); if( e != 0) { ui.updateEncoder( 3, e); }
    
    ui.tick();

    yield();
}