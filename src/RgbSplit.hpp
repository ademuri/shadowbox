#ifndef __RGB_SPLIT_HPP__
#define __RGB_SPLIT_HPP__

#include "Effect.hpp"
#include <SFML/Window.hpp>
#include <cv.h>

/** Effect that "splits" the red, green, and blue channels of the shadow of the
 * hand (by moving them slightly out of alignment with each other).
 */
class RgbSplit : public Effect {
public:
  RgbSplit(sf::RenderWindow *const window);

  void render(cv::Mat &frame) override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat output;
};

#endif
