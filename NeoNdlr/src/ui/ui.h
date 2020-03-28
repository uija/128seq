#ifndef _UI_H
#define _UI_H

#include "LiquidCrystal_I2C.h"
#include "Adafruit_NeoTrellis.h"
#include "uiscreen.h"
#include "uievent.h"

#define BRIGHTNESS 50

class UIScreen;

/**
 * UI Main Class
 */
class UI {
  Adafruit_MultiTrellis* _trellis;
  LiquidCrystal_I2C* _lcd;
  UIScreen* _currentScreen;
public:
  UI( Adafruit_MultiTrellis* trellis, LiquidCrystal_I2C* lcd);
  ~UI();
  void setScreen( UIScreen* screen);
  void updateEncoder( uint8_t idx, int8_t value);
  void buttonUp( uint8_t idx);
  void buttonDown( uint8_t idx);
  void tick();
  void forceUpdate();
};

#endif
