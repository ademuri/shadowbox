#ifndef __ROLLING_SHUTTER_HPP__
#define __ROLLING_SHUTTER_HPP__

#include "Effect.hpp"
#include <SDL2/SDL.h>
#include <cv.h>

/** Effect that has bands with increasing delay.
 */
class RollingShutter : public Effect {
public:
  RollingShutter(SDL_Renderer *const renderer_);

  void render(cv::Mat &frame) override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat output;

  unsigned int NUM_STRIPES = 10;
  unsigned int FRAMES_PER_STRIPE = 2;
  unsigned int BUFFER_SIZE = NUM_STRIPES * FRAMES_PER_STRIPE + 1;
  static const unsigned int ACTUAL_BUFFER_SIZE = 300;
  SDL_Texture *buffer[ACTUAL_BUFFER_SIZE];
  unsigned int index = 0;

  void renderFragment(int offset);

  void randomize() override;
};

#endif
