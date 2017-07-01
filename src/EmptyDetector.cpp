#include "EmptyDetector.hpp"
#include <cv.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

EmptyDetector::EmptyDetector() {}

// TODO: add tests for this functionality
bool EmptyDetector::coarseEmptyDetect(Mat &image) {
  runEveryCount++;
  if (runEveryCount < coarseEmptyDetectEvery) {
    return false;
  }

  runEveryCount = 0;
  if (fineEmptyDetect(image)) {
    emptyCount++;
  } else {
    emptyCount = 0;
  }

  if (emptyCount > coarseEmptyFrameThreshold) {
    return true;
  }

  return false;
}

bool EmptyDetector::fineEmptyDetect(Mat &image) {
  cvtColor(image, imageGray, cv::COLOR_RGB2GRAY);
  threshold(imageGray, thresh, 15, 255, cv::THRESH_BINARY);
  return countNonZero(thresh) < emptyFramePixelThreshold;
}
