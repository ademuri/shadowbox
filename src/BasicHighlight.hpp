#ifndef __BASIC_HIGHLIGHT_HPP__
#define __BASIC_HIGHLIGHT_HPP__

#include "Effect.hpp"
#include <SDL2/SDL.h>
#include <cv.h>

class BasicHighlight : public Effect {
public:
  BasicHighlight(SDL_Renderer *const renderer_);

  void render(const cv::Mat &frame) override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat output;
};

#endif
