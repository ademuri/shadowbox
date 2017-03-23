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

  static const unsigned int NUM_STRIPES = 3;
  static const unsigned int FRAMES_PER_STRIPE = 6;
  static const unsigned int BUFFER_SIZE = 20;
  SDL_Texture *buffer[BUFFER_SIZE];
  unsigned int index = 0;

  void renderFragment(int offset);
};

#endif
