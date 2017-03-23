#include "RollingShutter.hpp"
#include <cstdlib>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

RollingShutter::RollingShutter(SDL_Renderer *const renderer_)
    : Effect(renderer_) {
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);

  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = NULL;
  }
}

void RollingShutter::render(cv::Mat &frame) {
  // Highlight the hand in white, and make the rest of output black.
  findHand(frame, handMask, backMask);
  output.setTo(Scalar(0, 0, 0, 255), handMask);
  output.setTo(Scalar(255, 255, 255, 255), backMask);

  if (buffer[index] != NULL) {
    SDL_DestroyTexture(buffer[index]);
  }
  buffer[index] = createRGBATexture(output);

  renderFragment(2);
  renderFragment(1);
  renderFragment(0);

  SDL_RenderPresent(renderer);

  index = (index + 1) % BUFFER_SIZE;
}

void RollingShutter::renderFragment(int offset) {
  static const int OFFSET_HEIGHT = IMAGE_HEIGHT / NUM_STRIPES;

  SDL_Rect rect;
  rect.w = IMAGE_WIDTH;
  rect.h = OFFSET_HEIGHT;
  rect.x = 0;
  rect.y = OFFSET_HEIGHT * (NUM_STRIPES - offset - 1);

  const unsigned int bufferIndex =
      (index + BUFFER_SIZE - FRAMES_PER_STRIPE * offset) % BUFFER_SIZE;
  if (buffer[bufferIndex] != NULL) {
    SDL_RenderCopy(renderer, buffer[bufferIndex], &rect, &rect);
  }
}
