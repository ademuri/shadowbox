#ifndef __SIMPLE_STROBE_HPP__
#define __SIMPLE_STROBE_HPP__

#include "Effect.hpp"
#include "Projector.hpp"
#include <SDL2/SDL.h>
#include <cv.h>

class SimpleStrobe : public Effect {
public:
  SimpleStrobe(SDL_Renderer *const renderer_, Projector projector);

  void render(cv::Mat &frame) override;

  // void randomize() override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat output;
};

#endif
