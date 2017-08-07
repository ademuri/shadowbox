#ifndef __RGB_SPLIT_HPP__
#define __RGB_SPLIT_HPP__

#include "Effect.hpp"
#include "Projector.hpp"
#include <SDL2/SDL.h>
#include <cv.h>
#include <vector>

enum RgbSplitMode { RGB_SPLIT_FIXED, RGB_SPLIT_CENTER_OF_MASS };

/** Effect that "splits" the red, green, and blue channels of the shadow of the
 * hand (by moving them slightly out of alignment with each other).
 */
class RgbSplit : public Effect {
public:
  RgbSplit(SDL_Renderer *const renderer_, Projector projector);

  void render(cv::Mat &frame) override;

  void setMode(RgbSplitMode mode);
  void advanceMode() override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat output;

  long prevXCenter = 0;
  long prevYCenter = 0;
  RgbSplitMode mode;
  std::vector<RgbSplitMode> modes;
  unsigned int modeIndex = 0;
};

#endif
