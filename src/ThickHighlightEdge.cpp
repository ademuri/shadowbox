#include "ThickHighlightEdge.hpp"
#include <algorithm>
#include <cstdlib>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

ThickHighlightEdge::ThickHighlightEdge(SDL_Renderer *const renderer_)
    : Effect(renderer_) {
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
}

void ThickHighlightEdge::render(cv::Mat &frame) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  // Highlight the hand in white, and make the rest of output black.
  findHand(frame, handMask, backMask);

  output = Scalar(0, 0, 0, 255);

  Canny(handMask, edges, 50, 150);
  vector<vector<Point>> contours;
  findContours(edges, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

  // TODO: Switch to SDL/OpenGLES. This will require drawing using raw OpenGL
  // calls, because SDL doesn't support setting the line width.
  /*vector<SDL_Point> sdlPoints;
  unsigned int numLines = 0;
  for (auto i = contours.begin(); i != contours.end(); i++) {
    for (auto k = (*i).begin(); k != (*i).end(); k++) {
      sdlPoints.push_back(SDL_Point {k->x, k->y});
      numLines++;
    }
  }

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  if (numLines > 0) {
    numLines -= 1;
    SDL_RenderDrawLines(renderer, &sdlPoints.front(), numLines);
  }*/

  drawContours(output, contours, -1, Scalar(255, 255, 255, 255), 5);

  SDL_Texture *texture = createRGBATexture(output);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(texture);
}
