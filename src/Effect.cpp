#include "Effect.hpp"
#include "RgbUtil.hpp"
#include <SDL2/SDL.h>
#include <cv.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

// TODO: globals are bad
extern int thresholdFlag;

Effect::Effect(SDL_Renderer *const renderer_) : renderer(renderer_) {
  calcFramerateAt = SDL_GetTicks() + 1000;
  frames = 0;
}

Effect::Effect(SDL_Renderer *const renderer_, Projector projector)
    : renderer(renderer_), projector(projector) {
  calcFramerateAt = SDL_GetTicks() + 1000;
  frames = 0;
}

void Effect::findHand(const cv::Mat frame, cv::Mat &handMask,
                      cv::Mat &backMask) {
  cvtColor(frame, imageGray, cv::COLOR_RGB2GRAY);
  threshold(imageGray, handMask, thresholdFlag, 255, cv::THRESH_BINARY);
  bitwise_not(handMask, backMask);
}

SDL_Surface *Effect::createRGBSurface(cv::Mat &frame) const {
  return SDL_CreateRGBSurfaceFrom(frame.ptr(0), frame.size().width,
                                  frame.size().height,
                                  24,         // depth
                                  frame.step, // pitch
                                  0x000000ff, 0x0000ff00, 0x00ff0000, 0);
}

SDL_Surface *Effect::createRGBASurface(cv::Mat &frame) const {
  return SDL_CreateRGBSurfaceFrom(
      frame.ptr(0), frame.size().width, frame.size().height,
      32,         // depth
      frame.step, // pitch
      0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
}

SDL_Texture *Effect::createRGBTexture(cv::Mat &frame) {
  SDL_Surface *surface = createRGBSurface(frame);
  if (surface == nullptr) {
    return nullptr;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

SDL_Texture *Effect::createRGBATexture(cv::Mat &frame) {
  SDL_Surface *surface = createRGBASurface(frame);
  if (surface == nullptr) {
    return nullptr;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  return texture;
}

void Effect::renderMat(cv::Mat &frame) {
  SDL_Texture *tex = createRGBATexture(frame);
  if (tex == nullptr) {
    // TODO: make this common and re-add this
    // logSdlError("SDL_CreateTextureFromSurface Error: ");
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
}

void Effect::calculateFramerate() {
  Uint32 ticks = SDL_GetTicks();
  frames++;
  if (ticks > calcFramerateAt) {
    std::cout << "Framerate: " << frames << std::endl;

    // Calculate the framerate again in about 1s
    calcFramerateAt = SDL_GetTicks() + 999;
    frames = 0;
  }
}

void Effect::advanceMode() {
  // Default  behavior is no-op. Subclasses can override.
}

void Effect::setColorStep(unsigned int newColorStep) {
  colorStep = newColorStep;
  if (colorStepCounter > colorStep) {
    colorStepCounter = colorStep;
  }
}

RgbColor Effect::getNextColor() {
  switch (colorChangeMode) {
  case COLOR_CHANGE_SLOW:
    if (colorStepCounter == 0) {
      colorStepCounter = colorStep;
      currentColor.h++;
    }
    break;

  case COLOR_CHANGE_FAST:
    currentColor.h += colorStep;
    break;
  }

  if (saturationStep) {
    int next = currentColor.s + saturationStep;
    if (next > 255) {
      saturationStep = -saturationStep;
      next = 255;
    } else if (next < 0) {
      saturationStep = -saturationStep;
      next = 0;
    }
    currentColor.s = next;
  }

  colorStepCounter--;
  return RgbUtil::toRgb(currentColor);
}
