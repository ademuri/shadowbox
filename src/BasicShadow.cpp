#include "BasicShadow.hpp"
#include "Projector.hpp"
#include "RgbUtil.hpp"
#include <SDL2/SDL.h>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

BasicShadow::BasicShadow(SDL_Renderer *const renderer, Projector projector)
    : Effect(renderer, projector) {
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
}

// TODO: extract this into some sort of util
extern void logSdlError(const std::string &msg);

void BasicShadow::render(cv::Mat &frame) {
  foreground = getNextColor();

  // Shadow the hand in red, and make the rest of output transparent.
  findHand(frame, handMask, backMask);
  output.setTo(Scalar(foreground.r, foreground.g, foreground.b, 255), backMask);
  output.setTo(Scalar(0, 0, 0, 255), handMask);

  // Upload foreground image to the renderer
  SDL_Texture *tex = createRGBATexture(output);
  if (tex == nullptr) {
    logSdlError("SDL_CreateTextureFromSurface Error: ");
    return;
  }

  // First clear the renderer
  SDL_RenderClear(renderer);

  // Draw the textures
  SDL_RenderCopy(renderer, tex, NULL, NULL);

  // Update the screen
  SDL_RenderPresent(renderer);

  // Cleanup
  SDL_DestroyTexture(tex);

  projector.setColor(foreground);
}

void BasicShadow::randomize() {
  currentColor.h = rand() % 255;
  currentColor.s = rand() % 100 + 155;

  saturationStep = 0;

  switch (rand() % 6) {
  case 0:
  case 1:
    setColorStep(0);
    break;

  case 2:
    colorChangeMode = COLOR_CHANGE_SLOW;
    setColorStep(rand() % 5 + 1);
    break;

  case 3:
    colorChangeMode = COLOR_CHANGE_FAST;
    setColorStep(rand() % 10 + 1);
    break;

  // Change only saturation
  case 4:
    setColorStep(0);
    saturationStep = rand() % 10 + 1;
    break;

  // Change color and saturation
  case 5:
    setColorStep(rand() % 5 + 1);
    saturationStep = rand() % 10 + 1;
  }
}
