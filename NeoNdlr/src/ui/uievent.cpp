#include "uievent.h"

UIEvent::UIEvent() {
  _nextScreen = 0;
}
UIEvent::UIEvent( UIScreen* nextScreen) {
  _nextScreen = nextScreen;
}

UIScreen* UIEvent::getNextScreen() {
  return _nextScreen;
}
