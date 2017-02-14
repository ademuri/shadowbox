#include "BasicHighlight.hpp"
#include <SDL2/SDL.h>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

BasicHighlight::BasicHighlight(SDL_Renderer *const renderer)
    : Effect(renderer) {
  output.create(240, 320, CV_8UC4);
}

// TODO: extract this into some sort of util
extern void logSdlError(const std::string &msg);

void BasicHighlight::render(const cv::Mat &frame) {
  // Surface that holds the hand
  SDL_Surface *surface;

  // Surface that holds the image from the camera
  SDL_Surface *backgroundSurface;

  // Highlight the hand in red, and make the rest of output transparent.
  findHand(frame, handMask, backMask);
  output.setTo(Scalar(255, 0, 0, 255), backMask);
  output.setTo(Scalar(0, 0, 0, 0), handMask);

  // This holds the foreground (i.e. the hand)
  surface = SDL_CreateRGBSurfaceFrom(
      output.ptr(0), output.size().width, output.size().height,
      32,          // depth
      output.step, // pitch
      0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

  if (surface == nullptr) {
    logSdlError("Convert image failed: ");
    return;
  }

  // This holds the background (i.e. the raw image from the camera)
  backgroundSurface = SDL_CreateRGBSurfaceFrom(
      (void *)frame.ptr(0), frame.size().width, frame.size().height,
      24,         // depth
      frame.step, // pitch
      0x00ff0000, 0x0000ff00, 0x00ff0000, 0);

  if (backgroundSurface == nullptr) {
    logSdlError("Convert background image failed: ");
    return;
  }

  // Upload foreground image to the renderer
  SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  if (tex == nullptr) {
    logSdlError("SDL_CreateTextureFromSurface Error: ");
    return;
  }

  // Upload background image to the renderer
  SDL_Texture *backgroundTex =
      SDL_CreateTextureFromSurface(renderer, backgroundSurface);
  SDL_FreeSurface(backgroundSurface);
  if (backgroundTex == nullptr) {
    logSdlError("SDL_CreateTextureFromSurface Error: ");
    return;
  }

  // First clear the renderer
  SDL_RenderClear(renderer);

  // Draw the textures
  SDL_RenderCopy(renderer, backgroundTex, NULL, NULL);
  SDL_RenderCopy(renderer, tex, NULL, NULL);

  // Update the screen
  SDL_RenderPresent(renderer);

  // Cleanup
  SDL_DestroyTexture(backgroundTex);
  SDL_DestroyTexture(tex);
}
