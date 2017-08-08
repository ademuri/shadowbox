#include "BasicHighlight.hpp"
#include "Projector.hpp"
#include "RgbUtil.hpp"
#include <SDL2/SDL.h>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

BasicHighlight::BasicHighlight(SDL_Renderer *const renderer,
                               Projector projector)
    : Effect(renderer, projector) {
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
}

// TODO: extract this into some sort of util
extern void logSdlError(const std::string &msg);

void BasicHighlight::render(cv::Mat &frame) {
  // Highlight the hand in red, and make the rest of output transparent.
  findHand(frame, handMask, backMask);
  output.setTo(Scalar(red, green, blue, 255), handMask);
  output.setTo(Scalar(0, 0, 0, 0), backMask);

  // Upload foreground image to the renderer
  SDL_Texture *tex = createRGBATexture(output);
  if (tex == nullptr) {
    logSdlError("SDL_CreateTextureFromSurface Error: ");
    return;
  }

  // Upload background image to the renderer
  SDL_Texture *backgroundTex = createRGBTexture(frame);
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

  projector.setColor(red / 2, green / 2, blue / 2);
}

void BasicHighlight::randomize() {

  RgbColor color = RgbUtil::randomColor();
  red = color.r;
  green = color.g;
  blue = color.b;
}
