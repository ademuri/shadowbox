#include "BasicHighlight.hpp"
#include "Effect.hpp"
#include <SDL2/SDL.h>
#include <ctime>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void logSdlError(const std::string &msg) {
  std::cout << msg << "error: " << SDL_GetError() << std::endl;
}

int displaySdl() {
  VideoCapture cap;
  if (cap.open(0)) {
    std::cout << "Opened the camera." << std::endl;
  } else {
    std::cout << "Couldn't open the camera!" << std::endl;
    return 1;
  }

  if (!cap.set(CAP_PROP_FPS, 120)) {
    std::cout << "Couldn't set the FPS" << std::endl;
  }
  if (!cap.set(CAP_PROP_FRAME_WIDTH, 320)) {
    std::cout << "Couldn't set the width" << std::endl;
  }
  if (!cap.set(CAP_PROP_FRAME_HEIGHT, 240)) {
    std::cout << "Couldn't set the height" << std::endl;
  }
  if (!cap.set(CAP_PROP_AUTO_EXPOSURE, 0.25)) {
    std::cout << "couldn't set auto exposure" << std::endl;
  }
  // NOTE: this exposure value was chosen to make the hand light and the
  // background dark
  if (!cap.set(CAP_PROP_EXPOSURE, 0.005)) {
    std::cout << "couldn't set exposure" << std::endl;
  }

  // TODO: investigate capturing directly in grayscale
  /*if (!cap.set(CV_CAP_PROP_FORMAT, CV_8UC1)) {
    std::cout << "Couldn't set the format" << std::endl;
    }*/

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    logSdlError("SDL_Init Error: ");
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  atexit(SDL_Quit);

  // Make a window
  SDL_Window *win =
      SDL_CreateWindow("Hello World!", 100, 100, 320, 240, SDL_WINDOW_SHOWN);
  if (win == nullptr) {
    logSdlError("SDL_CreateWindow Error: ");
    return 1;
  }

  // Create a renderer
  SDL_Renderer *ren = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr) {
    SDL_DestroyWindow(win);
    logSdlError("SDL_CreateRenderer Error: ");
    return 1;
  }

  Mat image;

  Effect *effect = new BasicHighlight(ren);

  bool done = false;
  SDL_Event e;
  while (!done) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
      case SDL_KEYDOWN:
      case SDL_MOUSEBUTTONDOWN:
        done = true;
        break;
      default:
        // No-op
        break;
      }
    }
    if (done) {
      break;
    }

    cap.read(image);
    effect->render(image);
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  cap.release();
  return 0;
}

int main(int argc, char **argv) { return displaySdl(); }