#ifndef __BASIC_TRACER_HPP__
#define __BASIC_TRACER_HPP__

#include "Effect.hpp"
#include <SDL2/SDL.h>
#include <cv.h>

class BasicTracer : public Effect {
public:
  BasicTracer(SDL_Renderer *const renderer_);

  void render(cv::Mat &frame) override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat handImage;
  cv::Mat accumulator;
  cv::Mat output;
  SDL_Texture *backgroundTexture;
};

#endif
