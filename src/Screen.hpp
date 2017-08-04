#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

#include <ftdi.h>

class Screen {
public:
  virtual ~Screen() {}

  virtual void turnOff() = 0;

  virtual void turnOn() = 0;
};

class ScreenImpl : public Screen {
public:
  ScreenImpl();
  virtual ~ScreenImpl() {}

  virtual void turnOff();

  virtual void turnOn();

private:
  struct ftdi_context *ftdic;
};

#endif
