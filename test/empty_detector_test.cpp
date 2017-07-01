#include "EmptyDetector.hpp"
#include "gtest/gtest.h"
#include <cv.h>

using namespace cv;

TEST(EmptyDetectorTest, DetectsEmpty) {
  Mat empty;
  Mat full;

  empty.create(100, 100, CV_8UC3);
  full.create(100, 100, CV_8UC3);
  full = Scalar(255, 255, 255, 255);

  EmptyDetector detector = EmptyDetector(1, 10, 1, 10);

  for (int i = 0; i < 25; i++) {
    ASSERT_FALSE(detector.compute(full));
  }

  for (int i = 0; i < 19; i++) {
    ASSERT_FALSE(detector.compute(empty));
  }

  for (int i = 0; i < 25; i++) {
    ASSERT_TRUE(detector.compute(empty));
  }

  for (int i = 0; i < 25; i++) {
    ASSERT_FALSE(detector.compute(full));
  }
}

TEST(EmptyDetectorTest, CoarseRespectsRunEvery) {
  Mat empty;
  Mat full;

  empty.create(100, 100, CV_8UC3);
  full.create(100, 100, CV_8UC3);
  full = Scalar(255, 255, 255, 255);

  EmptyDetector detector = EmptyDetector(3, 1, 1, 1);

  // Test that we can pass a full and it gets skipped
  ASSERT_FALSE(detector.compute(full));
  ASSERT_FALSE(detector.compute(full));
  ASSERT_FALSE(detector.compute(empty));

  ASSERT_TRUE(detector.compute(empty));
}

TEST(EmptyDetectorTest, FineRespectsRunEvery) {
  Mat empty;
  Mat full;

  empty.create(100, 100, CV_8UC3);
  full.create(100, 100, CV_8UC3);
  full = Scalar(255, 255, 255, 255);

  EmptyDetector detector = EmptyDetector(1, 1, 3, 1);

  // Test that we can pass a full and it gets skipped
  ASSERT_FALSE(detector.compute(empty));

  ASSERT_FALSE(detector.compute(full));
  ASSERT_FALSE(detector.compute(full));
  ASSERT_TRUE(detector.compute(empty));
}
