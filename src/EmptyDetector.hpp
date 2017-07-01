#ifndef __EMPTY_DETECTOR_HPP__
#define __EMPTY_DETECTOR_HPP__

#include <cv.h>

using namespace cv;

enum EmptyDetectorState { ED_NORMAL, ED_EMPTY_COARSE, ED_EMPTY_FINE };
enum EmptyDetectorValue { ED_COUNTING, ED_EMPTY, ED_NOT_EMPTY };

class EmptyDetector {
public:
  EmptyDetector();

  /*
   * Constructor that allows specifying the tuning constants. Used for testing.
   */
  EmptyDetector(unsigned int coarseRunEvery, unsigned int coarseThreshold,
                unsigned int fineRunEvery, unsigned int fineThreshold);

  /*
   * Returns true if there's no foreground. Computes the hand mask, then counts
   * the number of pixels and compares it to a threshold.
   */
  bool compute(Mat &image);

private:
  // Require nothing every 60 frames for 5 seconds at 60FPS.
  const unsigned int coarseRunEvery = 60;
  const unsigned int coarseThreshold = 5;

  // Require nothing every frame for 5s at 60FPS.
  const unsigned int fineRunEvery = 1;
  const unsigned int fineThreshold = 300;

  // If there are fewer than this many pixels, consider the frame 'empty'.
  const int pixelThreshold = 100;

  Mat imageGray;
  Mat thresh;
  unsigned int emptyCount = 0;
  unsigned int runEveryCount = 0;

  EmptyDetectorState state = ED_NORMAL;

  bool detect(Mat &image);

  EmptyDetectorValue periodicDetect(Mat &image, unsigned int runEvery,
                                    unsigned int threshold);
};

#endif
