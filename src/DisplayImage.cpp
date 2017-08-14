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
#include "Projector.hpp"
#include "RgbSplit.hpp"
#include "RollingShutter.hpp"
#include "SimpleStrobe.hpp"
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

// If true, parse camera params from commandline flags. If false, get the effect
// number instead.
const bool cameraFlags = false;

int thresholdFlag = 25;

// Whether to change effects peroidically. Only disabled for testing.
const bool changeEffectFlag = true;

// Delay before changing effects, in seconds.
// TODO: add some randomness
// TODO: set this to an appropriate value
const unsigned int changeEffectEvery = 5;

using namespace cv;
using namespace std;

void logSdlError(const std::string &msg) {
  std::cout << msg << "error: " << SDL_GetError() << std::endl;
}

/*
 * Adds effect to  effects frequency times.
 */
void addEffect(vector<Effect *> *effects, unsigned int frequency,
               Effect *effect) {
  for (unsigned int i = 0; i < frequency; i++) {
    effects->push_back(effect);
  }
}

// Defaults: .009, .20, .80
int displaySdl(unsigned int effectFlag, float exposure, float gain,
               float contrast) {
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
  if (!cap.set(CAP_PROP_EXPOSURE, exposure)) {
    std::cout << "couldn't set exposure" << std::endl;
  }

  if (!cap.set(CAP_PROP_GAIN, gain)) {
    std::cout << "couldn't set gain" << std::endl;
  }

  if (!cap.set(CAP_PROP_CONTRAST, contrast)) {
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

  Projector projector;
  projector.setColor(0, 0, 0);

  Mat image;

  EmptyDetector emptyDetector;

  time_t changeEffectAt = time(nullptr) + changeEffectEvery;

  // Adjust the probability of each effect being randomly chosen by inserting
  // effects more than once.
  vector<Effect *> effects;
  // addEffect(&effects, 1, new BasicHighlight(ren, projector));
  addEffect(&effects, 10, new BasicHighlight(ren, projector));
  addEffect(&effects, 10, new BasicTracer(ren, projector));
  addEffect(&effects, 2,
            (new RgbSplit(ren, projector))->setMode(RGB_SPLIT_FIXED));
  addEffect(&effects, 3,
            (new RgbSplit(ren, projector))->setMode(RGB_SPLIT_CENTER_OF_MASS));
  addEffect(&effects, 3, new RollingShutter(ren));

  const unsigned int NUM_EFFECTS = effects.size();

  // TODO: clean these up and add these to the frequency map above
  /*effects[2] = new FlickerShadow(ren);
  effects[3] = new HighlightEdge(ren);
  effects[5] = new RollingShutter(ren);
  // TODO: fix this - it breaks other effects when switching from this one to
  // other effects
  effects[6] = new ThickHighlightEdge(ren, win);*/

  unsigned int effectIndex = effectFlag;
  if (effectIndex >= NUM_EFFECTS) {
    fprintf(stderr, "Effect num %d greater than number of effects (%d)\n",
            effectIndex, NUM_EFFECTS);
    effectIndex = 0;
  }

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
      projector.screenOffAnimationTick();
    } else {
      effects[effectIndex]->render(image);
      effects[effectIndex]->calculateFramerate();

      if (changeEffectFlag && time(nullptr) > changeEffectAt) {
        projector.setColor(0, 0, 0);
        effectIndex = rand() % NUM_EFFECTS;
        effects[effectIndex]->randomize();
        changeEffectAt = time(nullptr) + changeEffectEvery;
      }
    }
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  cap.release();
  return 0;
}

int main(int argc, char **argv) {
  const unsigned int defaultEffect = 0;

  if (cameraFlags) {
    if (argc == 1) {
      return displaySdl(defaultEffect, .011, .8, .7);
    } else if (argc == 5) {
      thresholdFlag = stof(argv[4]);
      return displaySdl(defaultEffect, stof(argv[1]), stof(argv[2]),
                        stof(argv[3]));
    } else {
      fprintf(stderr, "usage: %s <exposure> <gain> <contrast> <threshold>\n",
              argv[0]);
      return 1;
    }
  } else {
    if (argc == 1) {
      return displaySdl(defaultEffect, .011, .8, .7);
    } else if (argc == 2) {
      return displaySdl(stoi(argv[1]), .011, .8, .7);
    } else {
      fprintf(stderr, "usage: %s <effect>\n", argv[0]);
      return 1;
    }
  }
}
