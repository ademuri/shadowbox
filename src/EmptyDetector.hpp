#ifndef __EMPTY_DETECTOR_HPP__
#define __EMPTY_DETECTOR_HPP__

#include <cv.h>

using namespace cv;

class EmptyDetector {
public:
  EmptyDetector();

  /*
   * Returns true if it seems like there isn't anyone interacting with the
   * camera right now, which indicates that fine (i.e. every frame)
   * foreground detect should run. This function assumes that it's run
   * regularly, and only samples frames occasionally.
   *
   * TODO: consider using wall time instead of frame count
   */
  bool coarseEmptyDetect(Mat &image);

  /*
   * Returns true if there's no foreground. Computes the hand mask, then counts
   * the number of pixels and compares it to a threshold.
   */
  bool fineEmptyDetect(Mat &image);

private:
  // Require nothing every 60 frames for 5 seconds at 60FPS.
  const unsigned int coarseEmptyDetectEvery = 60;
  const unsigned int coarseEmptyFrameThreshold = 5;

  // Require nothing every frame for 5s at 60FPS.
  const unsigned int fineEmptyFrameThreshold = 300;

  // If there are fewer than this many pixels, consider the frame 'empty'.
  const int emptyFramePixelThreshold = 100;

  Mat imageGray;
  Mat thresh;
  unsigned int emptyCount = 0;
  unsigned int runEveryCount = 0;
};

#endif
