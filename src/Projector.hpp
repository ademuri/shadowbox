#ifndef __PROJECTOR_HPP__
#define __PROJECTOR_HPP__

#include "RgbUtil.hpp"
#include "fadecandy/opc_client.h"

/**
 * Controls the "projector" light that shines on the bottom of the screen.
 */
class Projector {
public:
  Projector();

  void setColor(unsigned char red, unsigned char green, unsigned char blue);

  void setColor(RgbColor color);

  void setEveryNColor(unsigned int n, unsigned int offset, RgbColor color);

  void setEveryNColor(unsigned int n, unsigned int offset, unsigned char red,
                      unsigned char green, unsigned char blue);

  /** Does an animation to indicate that the screen is "warming up" while it
   * initializes. Blocks for several seconds. */
  void animateScreenOn();

  /** Renders one frame of the screen off animation. */
  void screenOffAnimationTick();

private:
  OPCClient fadecandy;
  std::vector<uint8_t> packet;

  const unsigned int numPixels = 24;
};
#endif
