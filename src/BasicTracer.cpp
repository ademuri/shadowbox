#include "BasicTracer.hpp"
#include <SDL2/SDL.h>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// TODO: extract this into some sort of util
extern void logSdlError(const std::string &msg);

BasicTracer::BasicTracer(SDL_Renderer *const renderer) : Effect(renderer) {
  handImage.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
  accumulator.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
}

void BasicTracer::render(Mat &frame) {
  // Highlight the hand in red, and make the rest of output transparent.
  findHand(frame, handMask, backMask);
  handImage.setTo(Scalar(255, 0, 0, 255), handMask);
  handImage.setTo(Scalar(0, 0, 0, 0), backMask);

  addWeighted(handImage, 1.0, accumulator, 0.9, 0, output, -1);
  output.copyTo(accumulator);

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
}
