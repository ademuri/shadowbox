#include "Effect.hpp"
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
