#include "HighlightEdge.hpp"
#include <cstdlib>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

HighlightEdge::HighlightEdge(SDL_Renderer *const renderer_)
    : Effect(renderer_) {
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
  this->mode = HIGHLIGHT_EDGE_NORMAL;
}

void HighlightEdge::render(cv::Mat &frame) {
  // Highlight the hand in white, and make the rest of output black.
  findHand(frame, handMask, backMask);

  switch (this->mode) {
  case HIGHLIGHT_EDGE_NORMAL:
  default:
    output = Scalar(0, 0, 0, 255);
    break;

  case HIGHLIGHT_EDGE_FILL:
    output.setTo(Scalar(0, 0, 0, 255), backMask);
    break;
  }

  Canny(handMask, edges, 50, 150);
  output.setTo(Scalar(255, 255, 255, 255), edges);

  SDL_RenderClear(renderer);
  SDL_Texture *texture = createRGBATexture(output);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(texture);
}

void HighlightEdge::setMode(HighlightEdgeMode mode) { this->mode = mode; }
