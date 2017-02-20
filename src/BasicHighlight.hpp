#ifndef __BASIC_HIGHLIGHT_HPP__
#define __BASIC_HIGHLIGHT_HPP__

#include "Effect.hpp"
#include <SFML/Window.hpp>
#include <cv.h>

class BasicHighlight : public Effect {
public:
  BasicHighlight(sf::RenderWindow *const window);

  void render(cv::Mat &frame) override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat output;
};

#endif
