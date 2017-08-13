#include "RgbSplit.hpp"
#include <cstdlib>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;

RgbSplit::RgbSplit(SDL_Renderer *const renderer_, Projector projector)
    : Effect(renderer_, projector) {
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
  modes.push_back(RGB_SPLIT_FIXED);
  modes.push_back(RGB_SPLIT_CENTER_OF_MASS);
}

void RgbSplit::render(cv::Mat &frame) {
  // Highlight the hand in white, and make the rest of output transparent.
  findHand(frame, handMask, backMask);
  output.setTo(Scalar(255, 255, 255, 255), handMask);
  output.setTo(Scalar(0, 0, 0, 0), backMask);

  SDL_Texture *handTexture;

  long diff;
  long xCenter;
  long yCenter;
  cv::Moments moments;
  switch (this->mode) {
  case RGB_SPLIT_CENTER_OF_MASS:
    moments = cv::moments(handMask, true /* binaryImage */);
    xCenter = moments.m10 / moments.m00;
    yCenter = moments.m01 / moments.m00;
    diff = std::abs(xCenter - prevXCenter) + std::abs(yCenter - prevYCenter);

    prevXCenter = xCenter;
    prevYCenter = yCenter;
    break;

  case RGB_SPLIT_FIXED:
  default:
    diff = 5;
    break;
  }

  handTexture = createRGBATexture(output);
  SDL_Rect destRect;
  destRect.w = IMAGE_WIDTH;
  destRect.h = IMAGE_HEIGHT;

  SDL_SetTextureBlendMode(handTexture, SDL_BLENDMODE_ADD);

  SDL_SetTextureColorMod(handTexture, 255, 0, 0);
  destRect.x = -diff;
  destRect.y = 0;
  SDL_RenderCopy(renderer, handTexture, NULL, &destRect);

  SDL_SetTextureColorMod(handTexture, 0, 255, 0);
  destRect.x = diff;
  destRect.y = 0;
  SDL_RenderCopy(renderer, handTexture, NULL, &destRect);

  SDL_SetTextureColorMod(handTexture, 0, 0, 255);
  destRect.x = 0;
  destRect.y = diff;
  SDL_RenderCopy(renderer, handTexture, NULL, &destRect);

  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(handTexture);

  projector.stageEveryNColor(3, 0, 100, 0, 0);
  projector.stageEveryNColor(3, 1, 0, 100, 0);
  projector.stageEveryNColor(3, 2, 0, 0, 100);
  projector.show();
}

RgbSplit *RgbSplit::setMode(RgbSplitMode mode) {
  this->mode = mode;
  return this;
}

void RgbSplit::advanceMode() {
  modeIndex = (modeIndex + 1) % modes.size();
  setMode(modes[modeIndex]);
}
void RgbSplit::randomize() { advanceMode(); }
