#ifndef __HIGHLIGH_EDGE_HPP__
#define __HIGHLIGH_EDGE_HPP__

#include "Effect.hpp"
#include <SDL2/SDL.h>
#include <cv.h>

enum HighlightEdgeMode {
  // Highlight only the edges
  HIGHLIGHT_EDGE_NORMAL,
  // Highlight the edges and don't clear inside of the foreground, so that it
  // 'fills' with edges.
  HIGHLIGHT_EDGE_FILL
};

/** Effect that puts highlight lines on the edge of detected objects.
 */
class HighlightEdge : public Effect {
public:
  HighlightEdge(SDL_Renderer *const renderer_);

  void render(cv::Mat &frame) override;

  void setMode(HighlightEdgeMode mode);

private:
  cv::Mat handMask;
  cv::Mat backMask;
  cv::Mat output;
  cv::Mat edges;
  HighlightEdgeMode mode;
};

#endif
