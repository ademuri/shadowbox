#ifndef __SCREEN_HPP__
#define __SCREEN_HPP__

#include <ftdi.h>

class Screen {
public:
  Screen();

  void turnOff();

  void turnOn();

private:
  struct ftdi_context *ftdic;
};

#endif
