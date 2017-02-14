#include "Effect.hpp"
#include <SDL2/SDL.h>
#include <cv.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

Effect::Effect(SDL_Renderer *const renderer_) : renderer(renderer_) {}

void Effect::findHand(const cv::Mat frame, cv::Mat &handMask,
                      cv::Mat &backMask) {
  cvtColor(frame, imageGray, cv::COLOR_RGB2GRAY);
  threshold(imageGray, backMask, 50, 255, cv::THRESH_BINARY);
  bitwise_not(backMask, handMask);
}

SDL_Surface *Effect::createRGBSurface(cv::Mat &frame) const {
  return SDL_CreateRGBSurfaceFrom(frame.ptr(0), frame.size().width,
                                  frame.size().height,
                                  24,         // depth
                                  frame.step, // pitch
                                  0x000000ff, 0x0000ff00, 0x00ff0000, 0);
}

SDL_Surface *Effect::createRGBASurface(cv::Mat &frame) const {
  return SDL_CreateRGBSurfaceFrom(
      frame.ptr(0), frame.size().width, frame.size().height,
      32,         // depth
      frame.step, // pitch
      0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
}
