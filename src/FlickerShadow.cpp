#include "FlickerShadow.hpp"
#include <SDL2/SDL.h>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

FlickerShadow::FlickerShadow(SDL_Renderer *const renderer) : Effect(renderer) {
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);

  // Create a static, solid-color background
  background.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);
  background = Scalar(0, 0, 255, 0);

  flickerCountdown = 100;
}

// TODO: extract this into some sort of util
extern void logSdlError(const std::string &msg);

void FlickerShadow::render(cv::Mat &frame) {
  // Highlight the hand in red, and make the rest of output transparent.
  findHand(frame, handMask, backMask);

  flickerCountdown--;
  if (flickerCountdown <= 0) {
    output.setTo(Scalar(255, 0, 0, 255), handMask);
    flickerCountdown = (rand() % 100) + 10;
  } else {
    output.setTo(Scalar(0, 0, 0, 255), handMask);
  }
  output.setTo(Scalar(0, 0, 0, 0), backMask);

  // Upload foreground image to the renderer
  SDL_Texture *tex = createRGBATexture(output);
  if (tex == nullptr) {
    logSdlError("SDL_CreateTextureFromSurface Error: ");
    return;
  }

  // Upload background image to the renderer
  SDL_Texture *backgroundTex = createRGBTexture(background);
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
