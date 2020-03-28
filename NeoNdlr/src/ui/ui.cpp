#include "ui.h"


UI::UI( Adafruit_MultiTrellis* trellis, LiquidCrystal_I2C* lcd) {
  _trellis = trellis;
  _lcd = lcd;
  _currentScreen = 0;
}
UI::~UI() {
  if( _currentScreen != 0) {
    delete _currentScreen;
  }
}
void UI::updateEncoder( uint8_t idx, int8_t value) {
  if( _currentScreen != 0) {
    UIEvent* event = _currentScreen->onEncoderChanged( idx, value);
    if( event != 0) {
      UIScreen* nextScreen = event->getNextScreen();
      if( nextScreen != 0) {
        this->setScreen( nextScreen);
      }
      delete event;
    }
  }
}
void UI::forceUpdate() {
  if( _currentScreen != 0) {
    _currentScreen->forceUpdate();
  }
}
void UI::buttonUp( uint8_t idx) {
  if( _currentScreen != 0) {
    UIEvent* event = _currentScreen->onIndexPressed( idx);
    if( event != 0) {
      UIScreen* nextScreen = event->getNextScreen();
      if( nextScreen != 0) {
        this->setScreen( nextScreen);
      }
      delete event;
    }
  }
}
void UI::buttonDown( uint8_t idx) {
if( _currentScreen != 0) {
    UIEvent* event = _currentScreen->onIndexReleased( idx);
    if( event != 0) {
      UIScreen* nextScreen = event->getNextScreen();
      if( nextScreen != 0) {
        this->setScreen( nextScreen);
      }
      delete event;
    }
  }
}
void UI::tick() {
  if( _currentScreen != 0) {
    _currentScreen->render();
  }
}
void UI::setScreen( UIScreen* screen) {
  if( _currentScreen != 0) {
    delete _currentScreen;
  }
  _currentScreen = screen;
  _currentScreen->setRenderer( _trellis, _lcd);
  _currentScreen->render();
}
