#ifndef __BASIC_TRACER_HPP__
#define __BASIC_TRACER_HPP__

#include "Effect.hpp"
#include "Projector.hpp"
#include <SDL2/SDL.h>
#include <cv.h>

class BasicTracer : public Effect {
public:
  BasicTracer(SDL_Renderer *const renderer_, Projector projector);

  void render(cv::Mat &frame) override;

  void randomize();

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat handImage;
  SDL_Texture *accumulator;
  SDL_Texture *staging;
};

#endif
