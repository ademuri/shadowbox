#include "BasicHighlight.hpp"
#include "Effect.hpp"
#include <SDL_gpu_GLES_3.h>
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


  GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
  GPU_Target* window = GPU_Init(800, 600, GPU_DEFAULT_INIT_FLAGS);
  if (window == nullptr) {
    std::cout << "Unable to create window" << std::endl;
    return 1;
  }

  /*SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);*/

  atexit(GPU_Quit);

  // TODO: investigate what the best combination of these is for the screen
  /*if (SDL_GL_SetSwapInterval(0)) {
    logSdlError("Unable to disable VSync");
  }

  if (SDL_SetHint(SDL_HINT_RENDER_VSYNC, 0)) {
    logSdlError("Unable to disable VSync hint");
  }*/

  //Mat image;
  GPU_Image* image = GPU_LoadImage("smile.jpeg");
  if (image == nullptr) {
    std::cout << "Unable to load image" << std::endl;
    return 1;
  }

  //Effect *const effect = new BasicHighlight(window);

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

    GPU_ClearRGBA(window, 255, 255, 255, 255);
        // Remember that GPU_Blit() draws the image *centered* at the given position
        //GPU_Blit(image, NULL, window, image->w/2, image->h/2);
        // Show the result in the window
        GPU_Flip(window);

    /*(cap.read(image);
    effect->render(image);
    effect->calculateFramerate();*/
  }


  //GPU_Quit();
  cap.release();
  return 0;
}

int main(int argc, char **argv) { return displaySdl(); }
