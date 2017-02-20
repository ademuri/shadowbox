#include "BasicHighlight.hpp"
#include <SFML/Window.hpp>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

BasicHighlight::BasicHighlight(sf::RenderWindow *const window)
    : Effect(window) {
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
}

// TODO: extract this into some sort of util
extern void logSdlError(const std::string &msg);

void BasicHighlight::render(cv::Mat &frame) {
  // Highlight the hand in red, and make the rest of output transparent.
  findHand(frame, handMask, backMask);
  output.setTo(Scalar(255, 0, 0, 255), backMask);
  output.setTo(Scalar(0, 0, 0, 0), handMask);

  Mat frameRGBA;
  sf::Image bgImage;
  sf::Texture bgTexture;
  sf::Sprite bgSprite;

  sf::Image handImage;
  sf::Texture handTexture;
  sf::Sprite handSprite;

  cv::cvtColor(frame, frameRGBA, cv::COLOR_BGR2RGBA);
  createSfImage(frameRGBA, bgImage);
  bgTexture.loadFromImage(bgImage);
  bgSprite.setTexture(bgTexture);

  createSfImage(output, handImage);
  // TODO: error checking (throughout)
  handTexture.loadFromImage(handImage);
  handSprite.setTexture(handTexture);

  window->draw(bgSprite);
  window->draw(handSprite);
  window->display();
}
