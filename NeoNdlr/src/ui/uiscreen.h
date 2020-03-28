#ifndef _UISCREEN_H
#define _UISCREEN_H

#include <Arduino.h>
#include "Adafruit_NeoTrellis.h"
#include "LiquidCrystal_I2C.h"

class UIEvent;

/**
 * Screen
 */
class UIScreen {
protected:
  Adafruit_MultiTrellis* _trellis;
  LiquidCrystal_I2C* _lcd;
public:
  UIScreen();
  virtual ~UIScreen();
  void setRenderer( Adafruit_MultiTrellis* trellis, LiquidCrystal_I2C* lcd);
  virtual UIEvent* onIndexPressed( uint8_t index) { return 0; }
  virtual UIEvent* onIndexReleased( uint8_t index) { return 0; }
  virtual UIEvent* onEncoderChanged( uint8_t index, int8_t value) { return 0; }
  virtual void render() {}
  virtual void forceUpdate() {}
};

#endif
