#include <ctime>
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <SDL2/SDL.h>

using namespace cv;

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

  // TODO: investigate capturing directly in grayscale
  /*if (!cap.set(CV_CAP_PROP_FORMAT, CV_8UC1)) {
    std::cout << "Couldn't set the format" << std::endl;
    }*/

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  atexit(SDL_Quit);

  // Make a window
  SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 320, 240, SDL_WINDOW_SHOWN);
  if (win == nullptr){
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  // Create a renderer
  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr){
    SDL_DestroyWindow(win);
    std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  Mat image;
  Mat imageGray;
  Mat handMask;
  Mat backMask;
  SDL_Surface* surface;

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
    cvtColor(image, imageGray, COLOR_RGB2GRAY);

    threshold(imageGray, backMask, 20, 255, THRESH_BINARY | THRESH_OTSU);
    bitwise_not(backMask, handMask);

    image.setTo(Scalar(255, 0, 0), backMask);
    image.setTo(Scalar(0, 255, 0), handMask);

    surface = SDL_CreateRGBSurfaceFrom(image.ptr(0),
        image.size().width,
        image.size().height,
        24, // depth
        image.step,  // pitch
        0x00ff0000,
        0x000000ff,
        0x0000ff00,
        0);

    if (surface == nullptr) {
      printf("Convert image failed: %s\n", SDL_GetError());
    }

    // Upload image to the renderer
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    if (tex == nullptr){
      SDL_DestroyRenderer(ren);
      SDL_DestroyWindow(win);
      std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return 1;
    }

    //First clear the renderer
    SDL_RenderClear(ren);

    //Draw the texture
    SDL_RenderCopy(ren, tex, NULL, NULL);

    //Update the screen
    SDL_RenderPresent(ren);

    SDL_DestroyTexture(tex);
  }

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();

  cap.release();
  return 0;
}

int main( int argc, char** argv ) {
  return displaySdl();
}
