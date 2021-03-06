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

  for (unsigned int i = 0; i < ACTUAL_BUFFER_SIZE; i++) {
    buffer[i] = nullptr;
  }
}

void RollingShutter::render(cv::Mat &frame) {
  // Highlight the hand in white, and make the rest of output black.
  findHand(frame, handMask, backMask);
  output.setTo(Scalar(0, 0, 0, 255), backMask);
  output.setTo(Scalar(255, 255, 255, 255), handMask);

  if (buffer[index] != nullptr) {
    SDL_DestroyTexture(buffer[index]);
  }
  buffer[index] = createRGBATexture(output);

  for (int i = NUM_STRIPES - 1; i >= 0; i--) {
    renderFragment(i);
  }

  SDL_RenderPresent(renderer);

  index = (index + 1) % BUFFER_SIZE;
}

void RollingShutter::renderFragment(int offset) {
  const int OFFSET_HEIGHT = IMAGE_HEIGHT / NUM_STRIPES;

  SDL_Rect rect;
  rect.w = IMAGE_WIDTH;
  rect.h = OFFSET_HEIGHT;
  rect.x = 0;
  rect.y = OFFSET_HEIGHT * (NUM_STRIPES - offset - 1);

  const unsigned int bufferIndex =
      (index + BUFFER_SIZE - FRAMES_PER_STRIPE * offset) % BUFFER_SIZE;
  if (buffer[bufferIndex] != nullptr) {
    SDL_RenderCopy(renderer, buffer[bufferIndex], &rect, &rect);
  }
}

void RollingShutter::randomize() {
  for (unsigned int i = 0; i < BUFFER_SIZE; i++) {
    SDL_DestroyTexture(buffer[index]);
    buffer[i] = nullptr;
  }
  index = 0;

  switch (rand() % 2) {
  case 0:
    NUM_STRIPES = rand() % 10 + 2;
    FRAMES_PER_STRIPE = rand() % 10 + 1;
    break;

  case 1:
    NUM_STRIPES = rand() % 50 + 10;
    FRAMES_PER_STRIPE = rand() % 3 + 1;
    break;
  }
  BUFFER_SIZE = NUM_STRIPES * FRAMES_PER_STRIPE + 1;
  if (BUFFER_SIZE > ACTUAL_BUFFER_SIZE) {
    fprintf(stderr, "Warning: buffer too small! (%d, %d)\n", BUFFER_SIZE,
            ACTUAL_BUFFER_SIZE);
  }
}
