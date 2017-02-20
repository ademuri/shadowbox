#ifndef __CARTOON_HPP__
#define __CARTOON_HPP__

#include "Effect.hpp"
#include <SFML/Window.hpp>
#include <cv.h>

/** Makes the hand appear "cartoon"ish by eroding and dilating the image.
 */
class Cartoon : public Effect {
public:
  Cartoon(sf::RenderWindow *const window);

  void render(cv::Mat &frame) override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat output;
  sf::Shader shader;
};

#endif
