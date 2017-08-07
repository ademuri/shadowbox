#ifndef __PROJECTOR_HPP__
#define __PROJECTOR_HPP__

#include "fadecandy/opc_client.h"

/**
 * Controls the "projector" light that shines on the bottom of the screen.
 */
class Projector {
public:
  Projector();

  void setColor(unsigned char red, unsigned char green, unsigned char blue);

private:
  OPCClient fadecandy;
  std::vector<uint8_t> packet;

  const unsigned int numPixels = 24;
};
#endif
