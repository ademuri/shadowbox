#include "SimpleStrobe.hpp"
#include "Projector.hpp"
#include <SDL2/SDL.h>
#include <cv.h>
#include <iostream>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// TODO: extract this into some sort of util
extern void logSdlError(const std::string &msg);

SimpleStrobe::SimpleStrobe(SDL_Renderer *const renderer, Projector projector)
    : Effect(renderer, projector) {
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
}

void SimpleStrobe::render(cv::Mat &frame) {
  static unsigned int strobeCounter = 0;
  static bool strobeOn = true;
  bool prevStrobeOn = false;
  foreground = getNextColor();

  strobeCounter++;
  prevStrobeOn = strobeOn;
  if ((strobeOn && strobeCounter > strobeOnCount) ||
      (!strobeOn && strobeCounter > strobeOffCount)) {
    strobeOn = !strobeOn;
    strobeCounter = 0;
  }

  if (strobeOn) {
    if (!prevStrobeOn) {
      findHand(frame, handMask, backMask);
      output = Scalar(foreground.r, foreground.g, foreground.b, 255);
      output.setTo(Scalar(0, 0, 0, 255), handMask);
    }

    renderMat(output);
    projector.setColor(foreground);
  } else {
    output = Scalar(0, 0, 0, 255);
    renderMat(output);
    projector.setColor(0, 0, 0);
  }
}

void SimpleStrobe::randomize() {
  strobeOffCount = rand() % 20 + 10;
  strobeOnCount = rand() % 5 + 1;

  currentColor.h = rand() % 255;

  //  Either steady color or change color
  colorChangeMode = COLOR_CHANGE_SLOW;
  switch (rand() % 4) {
  // White
  case 0:
    currentColor.s = 0;
    setColorStep(0);
    break;

  // Single color
  case 1:
    currentColor.s = 255;
    setColorStep(0);
    break;

  // Change color
  case 2:
    currentColor.s = 255;
    setColorStep(rand() % 5 + 1);
    break;

  // Cycle through pastels
  case 3:
    currentColor.s = rand() % 30 + 20;
    setColorStep(2);
    break;
  }
}
