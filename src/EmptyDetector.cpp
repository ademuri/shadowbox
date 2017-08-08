#include "EmptyDetector.hpp"
#include "Projector.hpp"
#include "Screen.hpp"
#include <cv.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

extern int thresholdFlag;

EmptyDetector::EmptyDetector() {
  screen = new ScreenImpl();
  screen->turnOn();
}

// This is only used in tests, so don't initialize the screen.
// TODO: mock out the Screen object for test
EmptyDetector::EmptyDetector(Screen *screen, Projector projector,
                             unsigned int coarseRunEvery,
                             unsigned int coarseThreshold,
                             unsigned int fineRunEvery,
                             unsigned int fineThreshold)
    : coarseRunEvery(coarseRunEvery), coarseThreshold(coarseThreshold),
      fineRunEvery(fineRunEvery), fineThreshold(fineThreshold), screen(screen),
      projector(projector) {
  screen->turnOn();
}

bool EmptyDetector::detect(Mat &image) {
  cvtColor(image, imageGray, cv::COLOR_RGB2GRAY);
  threshold(imageGray, thresh, thresholdFlag, 255, cv::THRESH_BINARY);
  return countNonZero(thresh) < pixelThreshold;
}

// TODO: add tests for this functionality
EmptyDetectorValue EmptyDetector::periodicDetect(Mat &image,
                                                 unsigned int runEvery,
                                                 unsigned int threshold) {
  runEveryCount++;
  if (runEveryCount < runEvery) {
    return ED_COUNTING;
  }

  runEveryCount = 0;
  if (detect(image)) {
    emptyCount++;
  } else {
    emptyCount = 0;
    return ED_NOT_EMPTY;
  }

  if (emptyCount >= threshold) {
    return ED_EMPTY;
  }

  return ED_COUNTING;
}

bool EmptyDetector::compute(Mat &image) {
  EmptyDetectorValue value;

  // TODO: remove this to re-enable turning off the screen
  // return false;

  switch (state) {
  case ED_NORMAL:
    if (periodicDetect(image, coarseRunEvery, coarseThreshold) == ED_EMPTY) {
      state = ED_EMPTY_COARSE;
      emptyCount = 0;
    }
    break;

  case ED_EMPTY_COARSE:
    value = periodicDetect(image, fineRunEvery, fineThreshold);
    if (value == ED_EMPTY) {
      screen->turnOff();

      state = ED_EMPTY_FINE;
      return true;
    } else if (value == ED_NOT_EMPTY) {
      state = ED_NORMAL;
      emptyCount = 0;
    }
    break;

  case ED_EMPTY_FINE:
    if (detect(image)) {
      return true;
    }
    state = ED_NORMAL;
    emptyCount = 0;
    screen->turnOn();
    projector.animateScreenOn();
    break;

  default:
    cout << "Warning: invalid EmptyDetector state: " << state << endl;
    return false;
  }

  return false;
}
