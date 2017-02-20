#include "FlickerShadow.hpp"
#include <SDL2/SDL.h>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

FlickerShadow::FlickerShadow(sf::RenderWindow *const window) : Effect(window) {
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);

  // Create a static, solid-color background
  background.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
  background = Scalar(0, 0, 255, 255);

  flickerCountdown = 100;
}

void FlickerShadow::render(cv::Mat &frame) {
  // Highlight the hand in red, and make the rest of output transparent.
  findHand(frame, handMask, backMask);

  flickerCountdown--;
  if (flickerCountdown <= 0) {
    output.setTo(Scalar(255, 0, 0, 255), backMask);
    flickerCountdown = (rand() % 100) + 10;
  } else {
    output.setTo(Scalar(0, 0, 0, 255), backMask);
  }
  output.setTo(Scalar(0, 0, 0, 0), handMask);

  sf::Image bgImage;
  sf::Texture bgTexture;
  sf::Sprite bgSprite;

  sf::Image handImage;
  sf::Texture handTexture;
  sf::Sprite handSprite;

  createSfImage(background, bgImage);
  bgTexture.loadFromImage(bgImage);
  bgSprite.setTexture(bgTexture);

  createSfImage(output, handImage);
  handTexture.loadFromImage(handImage);
  handSprite.setTexture(handTexture);

  window->draw(bgSprite);
  window->draw(handSprite);
  window->display();
}
