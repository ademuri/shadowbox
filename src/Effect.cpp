#include "Effect.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cv.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

Effect::Effect(sf::RenderWindow *const window) : window(window) { frames = 0; }

void Effect::findHand(const cv::Mat frame, cv::Mat &handMask,
                      cv::Mat &backMask) {
  cvtColor(frame, imageGray, cv::COLOR_RGB2GRAY);
  threshold(imageGray, backMask, 50, 255, cv::THRESH_BINARY);
  bitwise_not(backMask, handMask);
}

void Effect::createSfImage(cv::Mat frame, sf::Image &image) {
  image.create(frame.cols, frame.rows, frame.ptr());
}

void Effect::calculateFramerate() {
  frames++;

  if (clock.getElapsedTime().asMilliseconds() < 1000) {
    return;
  }
  clock.restart();

  std::cout << "Framerate: " << frames << std::endl;
  frames = 0;
}
