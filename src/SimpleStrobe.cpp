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

const unsigned int STROBE_OFF_COUNT = 20;
const unsigned int STROBE_ON_COUNT = 4;

void SimpleStrobe::render(cv::Mat &frame) {
  static unsigned int strobeCounter = 0;
  static bool strobeOn = true;
  bool prevStrobeOn = false;

  strobeCounter++;
  prevStrobeOn = strobeOn;
  if ((strobeOn && strobeCounter > STROBE_ON_COUNT) ||
      (!strobeOn && strobeCounter > STROBE_OFF_COUNT)) {
    strobeOn = !strobeOn;
    strobeCounter = 0;
  }

  if (strobeOn) {
    if (!prevStrobeOn) {
      findHand(frame, handMask, backMask);
      output = Scalar(255, 255, 255, 255);
      output.setTo(Scalar(0, 0, 0, 255), handMask);
    }

    renderMat(output);
    projector.setColor(255, 255, 255);
  } else {
    output = Scalar(0, 0, 0, 255);
    renderMat(output);
    projector.setColor(0, 0, 0);
  }
}
