#include "BasicTracer.hpp"
#include "Projector.hpp"
#include <SDL2/SDL.h>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// TODO: extract this into some sort of util
extern void logSdlError(const std::string &msg);

BasicTracer::BasicTracer(SDL_Renderer *const renderer, Projector projector)
    : Effect(renderer, projector) {
  handImage.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
  accumulator =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_TARGET, IMAGE_WIDTH, IMAGE_HEIGHT);
  staging =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_TARGET, IMAGE_WIDTH, IMAGE_HEIGHT);

  SDL_SetTextureBlendMode(accumulator, SDL_BLENDMODE_BLEND);
  SDL_SetTextureBlendMode(staging, SDL_BLENDMODE_BLEND);
}

void BasicTracer::render(Mat &frame) {
  foreground = getNextColor();

  // Highlight the hand in red, and make the rest of output transparent.
  findHand(frame, handMask, backMask);
  handImage.setTo(Scalar(foreground.r, foreground.g, foreground.b, 255),
                  handMask);
  handImage.setTo(Scalar(0, 0, 0, 0), backMask);

  SDL_SetRenderTarget(renderer, staging);
  SDL_RenderClear(renderer);
  SDL_Texture *tex = createRGBATexture(handImage);
  if (tex == nullptr) {
    logSdlError("SDL_CreateTextureFromSurface Error: ");
    return;
  }

  if (SDL_SetTextureAlphaMod(accumulator, tracerGain)) {
    logSdlError("SetTextureAlphaMod Error: ");
  }
  if (SDL_SetTextureAlphaMod(staging, 255)) {
    logSdlError("SetTextureAlphaMod Error: ");
  }
  SDL_RenderCopy(renderer, accumulator, NULL, NULL);
  SDL_RenderCopy(renderer, tex, NULL, NULL);

  // Copy the output back to the accumulator
  SDL_SetTextureAlphaMod(accumulator, 255);
  SDL_SetRenderTarget(renderer, accumulator);
  SDL_RenderCopy(renderer, staging, NULL, NULL);

  // Set the renderer back to the screen
  SDL_SetRenderTarget(renderer, NULL);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, staging, NULL, NULL);
  SDL_RenderPresent(renderer);
  SDL_DestroyTexture(tex);

  // TODO: do something cooler?
  projector.setColor(foreground);
}

void BasicTracer::randomize() {
  // Clear the accumulator
  SDL_SetRenderTarget(renderer, accumulator);
  SDL_RenderClear(renderer);
  // Set the renderer back to the screen
  SDL_SetRenderTarget(renderer, NULL);

  // Random in the range of 220-254
  // Note: if this is 255, the screen will never clear by itself
  tracerGain = rand() % 35 + 220;

  switch (rand() % 6) {
  case 0:
  case 1:
    setColorStep(0);
    break;

  // Slow change
  case 2:
  case 3:
    colorChangeMode = COLOR_CHANGE_SLOW;
    setColorStep(rand() % 5 + 1);
    break;

  // Fast change
  case 4:
    colorChangeMode = COLOR_CHANGE_FAST;
    setColorStep(rand() % 50 + 1);
    break;

  // Paint
  case 5:
    tracerGain = 255;
    if (rand() % 2) {
      setColorStep(rand() % 5 + 1);
      colorChangeMode = COLOR_CHANGE_SLOW;

    } else {
      colorChangeMode = COLOR_CHANGE_FAST;
      setColorStep(rand() % 50 + 1);
    }
    break;
  }
}
