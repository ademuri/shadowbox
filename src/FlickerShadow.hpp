#ifndef __FLICKER_SHADOW_HPP__
#define __FLICKER_SHADOW_HPP__

#include "Effect.hpp"
#include <SFML/Window.hpp>
#include <cv.h>

/** Renderer that draws a solid background with a shadow of the hand
 * that "flickers" on randomly.
 */
class FlickerShadow : public Effect {
public:
  FlickerShadow(sf::RenderWindow *const window);

  void render(cv::Mat &frame) override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat output;
  cv::Mat background;
  double flickerCountdown;
};

#endif
