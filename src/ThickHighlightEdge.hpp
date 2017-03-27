#ifndef __THICK_HIGHLIGHT_EDGE_HPP__
#define __THICK_HIGHLIGHT_EDGE_HPP__

#include "Effect.hpp"
#include <SDL2/SDL.h>
#include <cv.h>

/** Effect that puts highlight lines on the edge of detected objects.
 */
class ThickHighlightEdge : public Effect {
public:
  ThickHighlightEdge(SDL_Renderer *const renderer_);

  void render(cv::Mat &frame) override;

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat output;
  cv::Mat edges;
};

#endif
