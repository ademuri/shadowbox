#include "RgbSplit.hpp"
#include <SFML/Window.hpp>
#include <cstdlib>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

RgbSplit::RgbSplit(sf::RenderWindow *const window) : Effect(window) {
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
}

void RgbSplit::render(cv::Mat &frame) {
  // Highlight the hand in white, and make the rest of output transparent.
  findHand(frame, handMask, backMask);
  output.setTo(Scalar(255, 255, 255, 255), backMask);
  output.setTo(Scalar(0, 0, 0, 0), handMask);

  sf::Image handImage;
  sf::Texture handTexture;
  sf::Sprite redSprite;
  sf::Sprite greenSprite;
  sf::Sprite blueSprite;

  long diff;
  long xCenter;
  long yCenter;
  cv::Moments moments;
  switch (this->mode) {
  case RGB_SPLIT_CENTER_OF_MASS:
    moments = cv::moments(backMask, true /* binaryImage */);
    xCenter = moments.m10 / moments.m00;
    yCenter = moments.m01 / moments.m00;
    diff = std::abs(xCenter - prevXCenter) + std::abs(yCenter - prevYCenter);

    prevXCenter = xCenter;
    prevYCenter = yCenter;
    break;

  case RGB_SPLIT_FIXED:
  default:
    diff = 5;
    break;
  }

  createSfImage(output, handImage);
  handTexture.loadFromImage(handImage);
  redSprite.setTexture(handTexture);
  redSprite.setColor(sf::Color::Red);
  redSprite.move(-diff, 0);

  greenSprite.setTexture(handTexture);
  greenSprite.setColor(sf::Color::Green);
  greenSprite.move(diff, 0);

  blueSprite.setTexture(handTexture);
  blueSprite.setColor(sf::Color::Blue);
  blueSprite.move(0, diff);

  sf::RenderStates add = sf::RenderStates(sf::BlendAdd);
  window->clear(sf::Color::Black);
  window->draw(redSprite, add);
  window->draw(greenSprite, add);
  window->draw(blueSprite, add);
  window->display();
}

void RgbSplit::setMode(RgbSplitMode mode) { this->mode = mode; }
