#include "uiscreen.h"

UIScreen::UIScreen() {
  _trellis = 0;
}
UIScreen::~UIScreen() {
}
void UIScreen::setRenderer( Adafruit_MultiTrellis* trellis, LiquidCrystal_I2C* lcd) {
  _trellis = trellis;
  _lcd = lcd;
  _lcd->clear();
  for( uint8_t i = 0; i < 128; ++i) {
    _trellis->setPixelColor( i, 0x000000);
  }
  _trellis->show();
}
