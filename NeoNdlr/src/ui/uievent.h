#ifndef _UIEVENT_H
#define _UIEVENT_H

class UIScreen;

class UIEvent {
  UIScreen* _nextScreen;
public:
  UIEvent();
  UIEvent( UIScreen* nextScreen);
  UIScreen* getNextScreen();
};

#endif
