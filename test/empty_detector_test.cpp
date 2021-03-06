#include "EmptyDetector.hpp"
#include "Screen.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <cv.h>

using namespace cv;

int thresholdFlag = 25;

class MockScreen : public Screen {
public:
  MockScreen() {}
  MOCK_METHOD0(turnOff, void());
  MOCK_METHOD0(turnOn, void());
};

TEST(EmptyDetectorTest, DetectsEmpty) {
  Mat empty;
  Mat full;
  MockScreen screen;
  EXPECT_CALL(screen, turnOff());
  EXPECT_CALL(screen, turnOn()).Times(2);

  empty.create(100, 100, CV_8UC3);
  full.create(100, 100, CV_8UC3);
  full = Scalar(255, 255, 255, 255);

  EmptyDetector detector = EmptyDetector(&screen, 1, 10, 1, 10);

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
  MockScreen screen;
  EXPECT_CALL(screen, turnOff());
  EXPECT_CALL(screen, turnOn());

  empty.create(100, 100, CV_8UC3);
  full.create(100, 100, CV_8UC3);
  full = Scalar(255, 255, 255, 255);

  EmptyDetector detector = EmptyDetector(&screen, 3, 1, 1, 1);

  // Test that we can pass a full and it gets skipped
  ASSERT_FALSE(detector.compute(full));
  ASSERT_FALSE(detector.compute(full));
  ASSERT_FALSE(detector.compute(empty));

  ASSERT_TRUE(detector.compute(empty));
}

TEST(EmptyDetectorTest, FineRespectsRunEvery) {
  Mat empty;
  Mat full;
  MockScreen screen;
  EXPECT_CALL(screen, turnOff());
  EXPECT_CALL(screen, turnOn());

  empty.create(100, 100, CV_8UC3);
  full.create(100, 100, CV_8UC3);
  full = Scalar(255, 255, 255, 255);

  EmptyDetector detector = EmptyDetector(&screen, 1, 1, 3, 1);

  // Test that we can pass a full and it gets skipped
  ASSERT_FALSE(detector.compute(empty));

  ASSERT_FALSE(detector.compute(full));
  ASSERT_FALSE(detector.compute(full));
  ASSERT_TRUE(detector.compute(empty));
}

TEST(EmptyDetectorTest, RespectsPixelThreshold) {
  Mat empty;
  Mat partial;
  MockScreen screen;
  EXPECT_CALL(screen, turnOff());
  EXPECT_CALL(screen, turnOn()).Times(2);

  empty.create(99, 99, CV_8UC3);
  partial.create(99, 99, CV_8UC3);
  partial = Scalar(0, 0, 0);

  EmptyDetector detector = EmptyDetector(&screen, 1, 1, 1, 1);

  // Pixel threshold is 100 pixels
  partial.row(0) = Scalar(255, 255, 255);

  ASSERT_FALSE(detector.compute(partial));
  for (int i = 0; i < 10; i++) {
    ASSERT_TRUE(detector.compute(partial));
  }

  partial.row(1) = Scalar(255, 255, 255);

  for (int i = 0; i < 10; i++) {
    ASSERT_FALSE(detector.compute(partial));
  }
}
