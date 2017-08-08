#ifndef __BASIC_HIGHLIGHT_HPP__
#define __BASIC_HIGHLIGHT_HPP__

#include "Effect.hpp"
#include "Projector.hpp"
#include <SDL2/SDL.h>
#include <cv.h>

class BasicHighlight : public Effect {
public:
  BasicHighlight(SDL_Renderer *const renderer_, Projector projector);

  void render(cv::Mat &frame) override;

  void randomize() override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat output;
};

#endif
