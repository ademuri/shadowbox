#define GL_GLEXT_PROTOTYPES 1

#include "ThickHighlightEdge.hpp"
#include "rendering/Renderer.hpp"
#include <GLES3/gl31.h>
#include <algorithm>
#include <cstdlib>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

ThickHighlightEdge::ThickHighlightEdge(SDL_Renderer *const renderer_,
                                       SDL_Window *const win_)
    : Effect(renderer_) {
  win = win_;
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
}

void ThickHighlightEdge::render(cv::Mat &frame) {
  // Highlight the hand in white, and make the rest of output black.
  findHand(frame, handMask, backMask);

  output = Scalar(0, 0, 0, 255);

  Canny(handMask, edges, 50, 150);
  vector<vector<Point>> contours;
  findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

  // Clear the color buffer
  glClear(GL_COLOR_BUFFER_BIT);

  Renderer::useSolidShader();
  glLineWidth(5.0);
  Renderer::setSolidShaderColor(1.0, 1.0, 1.0, 1.0);

  vector<GlPoint> points;
  for (auto i = contours.begin(); i != contours.end(); i++) {
    for (auto k = (*i).begin(); k != (*i).end(); k++) {
      points.push_back(Renderer::pixelsToPoint(k->x, k->y));
    }
  }

  // Load the vertex data
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, points.data());
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_POINTS, 0, points.size());

  SDL_GL_SwapWindow(win);
}
