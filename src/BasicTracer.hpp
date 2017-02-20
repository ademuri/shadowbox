#ifndef __BASIC_TRACER_HPP__
#define __BASIC_TRACER_HPP__

#include "Effect.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cv.h>

class BasicTracer : public Effect {
public:
  BasicTracer(sf::RenderWindow *const window);

  void render(cv::Mat &frame) override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat handImage;
  cv::Mat accumulator;
  cv::Mat output;
};

#endif
