// Define this property so that we get the prototypes, and then inclue gl31.h
// first so that the other includes also have the define.
#define GL_GLEXT_PROTOTYPES 1
#include <GLES3/gl31.h>

#include "BasicHighlight.hpp"
#include "BasicTracer.hpp"
#include "Effect.hpp"
#include "EmptyDetector.hpp"
#include "FlickerShadow.hpp"
#include "HighlightEdge.hpp"
#include "RgbSplit.hpp"
#include "RollingShutter.hpp"
#include "ThickHighlightEdge.hpp"
#include "rendering/Renderer.hpp"
#include <SDL2/SDL.h>
#include <ctime>
#include <cv.h>
#include <ftdi.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

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

  // Note: these properties appear to persist across power cycling the camera,
  // so explicitly set all of the properties that we care about, just in case.
  // Note: you can use 'v4l2ucp' to play with these values in real-time.
  if (!cap.set(CAP_PROP_FPS, 120)) {
    std::cout << "Couldn't set the FPS" << std::endl;
  }
  if (!cap.set(CAP_PROP_FRAME_WIDTH, 240)) {
    std::cout << "Couldn't set the width" << std::endl;
  }
  if (!cap.set(CAP_PROP_FRAME_HEIGHT, 240)) {
    std::cout << "Couldn't set the height" << std::endl;
  }

  // Manual exposure
  if (!cap.set(CAP_PROP_AUTO_EXPOSURE, 0.25)) {
    std::cout << "couldn't set auto exposure" << std::endl;
  }
  // NOTE: this exposure value was chosen to make the hand light and the
  // background dark
  if (!cap.set(CAP_PROP_EXPOSURE, .004)) {
    std::cout << "couldn't set exposure" << std::endl;
  }

  if (!cap.set(CAP_PROP_GAIN, 0.60)) {
    std::cout << "couldn't set gain" << std::endl;
  }

  if (!cap.set(CAP_PROP_CONTRAST, .70)) {
    std::cout << "couldn't set contrast" << std::endl;
  }

  if (!cap.set(CAP_PROP_BRIGHTNESS, .50)) {
    std::cout << "couldn't set brightness" << std::endl;
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
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  atexit(SDL_Quit);

  // Make a window
  // These coordinates are chosen to work with the following offests for the
  // screen area of the LED screen:
  // Start X: 18
  // Start Y: 135
  // (This is dependent on hardware and was determined through trial and error).
  SDL_Window *const win = SDL_CreateWindow(
      "Hello World!", 50, 30, 240, 320, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  if (win == nullptr) {
    logSdlError("SDL_CreateWindow Error: ");
    return 1;
  }

  SDL_GLContext context = SDL_GL_CreateContext(win);
  if (context == nullptr) {
    logSdlError("SDL_GL_CreateContext: ");
    return 1;
  }
  const GLubyte *version = glGetString(GL_VERSION);
  if (version == 0) {
    logSdlError("GL Version: ");
    return 1;
  }
  std::cout << "Version: " << version << std::endl;

  // Create a renderer
  SDL_Renderer *const ren =
      SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (ren == nullptr) {
    SDL_DestroyWindow(win);
    logSdlError("SDL_CreateRenderer Error: ");
    return 1;
  }

  // TODO: investigate what the best combination of these is for the screen
  if (SDL_GL_SetSwapInterval(0)) {
    logSdlError("Unable to disable VSync");
  }

  if (SDL_SetHint(SDL_HINT_RENDER_VSYNC, 0)) {
    logSdlError("Unable to disable VSync hint");
  }

  // Initialize the shaders.
  Renderer::Init();

  Mat image;

  EmptyDetector emptyDetector;

  // Make an array of all of the effects, so that the user can switch between
  // them with the left and right arrows.
  const unsigned int NUM_EFFECTS = 7;
  Effect *effects[NUM_EFFECTS];
  effects[0] = new BasicHighlight(ren);
  effects[1] = new BasicTracer(ren);
  effects[2] = new FlickerShadow(ren);
  effects[3] = new HighlightEdge(ren);
  effects[4] = new RgbSplit(ren);
  effects[5] = new RollingShutter(ren);
  effects[6] = new ThickHighlightEdge(ren, win);

  int effectIndex = 0;

  bool done = false;
  SDL_Event e;
  while (!done) {
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
      case SDL_MOUSEBUTTONDOWN:
        done = true;
        break;

      case SDL_KEYUP:
        switch (e.key.keysym.sym) {
        case SDLK_LEFT:
          effectIndex = (effectIndex - 1 + NUM_EFFECTS) % NUM_EFFECTS;
          break;

        case SDLK_RIGHT:
          effectIndex = (effectIndex + 1) % NUM_EFFECTS;
          break;

        case SDLK_UP:
          effects[effectIndex]->advanceMode();
          break;

        case SDLK_ESCAPE:
          done = true;
          break;
        }
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
    // The image comes in mirrored and rotated, so fix it.
    rotate(image, image, ROTATE_90_COUNTERCLOCKWISE);
    flip(image, image, 1);

    if (emptyDetector.compute(image)) {
      // TODO: anything to do here?
    } else {
      effects[effectIndex]->render(image);
      effects[effectIndex]->calculateFramerate();
    }
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  cap.release();
  return 0;
}

int main(int argc, char **argv) { return displaySdl(); }
