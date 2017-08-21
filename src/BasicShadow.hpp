#ifndef __BASIC_SHADOW_HPP__
#define __BASIC_SHADOW_HPP__

#include "Effect.hpp"
#include "Projector.hpp"
#include <SDL2/SDL.h>
#include <cv.h>

class BasicShadow : public Effect {
public:
  BasicShadow(SDL_Renderer *const renderer_, Projector projector);

  void render(cv::Mat &frame) override;

  void randomize() override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat output;
};

#endif
