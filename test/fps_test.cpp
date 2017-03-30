#include "BasicHighlight.hpp"
#include "BasicTracer.hpp"
#include "FlickerShadow.hpp"
#include "HighlightEdge.hpp"
#include "RgbSplit.hpp"
#include "RollingShutter.hpp"
#include "ThickHighlightEdge.hpp"
#include "rendering/Renderer.hpp"
#include "gtest/gtest.h"
#include <GLES3/gl31.h>
#include <SDL2/SDL.h>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void logSdlError(const std::string &msg) { FAIL() << msg << SDL_GetError(); }

void testFps(Effect *effect, const std::string effectName,
             unsigned int minFps = 50) {
  VideoCapture cap;
  ASSERT_TRUE(cap.open("test/assets/hand1.avi"));

  Mat image;

  Uint32 startTicks = SDL_GetTicks();
  unsigned int frames = 0;

  while (cap.read(image)) {
    effect->render(image);
    frames++;
  }

  unsigned int fps = ((double)frames) / (SDL_GetTicks() - startTicks) * 1000.0;
  EXPECT_GE(fps, minFps) << effectName;
  std::cout << effectName << " FPS: " << fps << std::endl;
}

TEST(FpsTest, FpsIsHighEnough) {
  // REQUIRE( Factorial(1) == 1 );
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    logSdlError("SDL_Init Error: ");
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  atexit(SDL_Quit);
  // Make a window
  SDL_Window *const win = SDL_CreateWindow(
      "Hello World!", 300, 100, 320, 240, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  if (win == nullptr) {
    logSdlError("SDL_CreateWindow Error: ");
  }

  SDL_GLContext context = SDL_GL_CreateContext(win);
  if (context == nullptr) {
    logSdlError("SDL_GL_CreateContext: ");
  }

  // Create a renderer
  SDL_Renderer *const ren =
      SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  if (ren == nullptr) {
    SDL_DestroyWindow(win);
    logSdlError("SDL_CreateRenderer Error: ");
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

  BasicHighlight *basicHighlight = new BasicHighlight(ren);
  testFps(basicHighlight, "BasicHiglight");

  BasicTracer *basicTracer = new BasicTracer(ren);
  testFps(basicTracer, "BasicTracer");

  FlickerShadow *flickerShadow = new FlickerShadow(ren);
  testFps(flickerShadow, "FlickerShadow");

  HighlightEdge *highlightEdge = new HighlightEdge(ren);
  highlightEdge->setMode(HIGHLIGHT_EDGE_NORMAL);
  testFps(highlightEdge, "HighlightEdge-NORMAL", 45);
  highlightEdge->setMode(HIGHLIGHT_EDGE_FILL);
  testFps(highlightEdge, "HighlightEdge-FILL", 45);

  RgbSplit *rgbSplit = new RgbSplit(ren);
  rgbSplit->setMode(RGB_SPLIT_FIXED);
  testFps(rgbSplit, "RgbSplit-FIXED");
  rgbSplit->setMode(RGB_SPLIT_CENTER_OF_MASS);
  testFps(rgbSplit, "RgbSplit-CENTER_OF_MASS");

  RollingShutter *rollingShutter = new RollingShutter(ren);
  testFps(rollingShutter, "RollingShutter");

  ThickHighlightEdge *thickHighlightEdge = new ThickHighlightEdge(ren, win);
  testFps(thickHighlightEdge, "ThickHighlightEdge");
}
