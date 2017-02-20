#include "RgbSplit.hpp"
#include <SFML/Window.hpp>
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

  createSfImage(output, handImage);
  handTexture.loadFromImage(handImage);
  redSprite.setTexture(handTexture);
  redSprite.setColor(sf::Color::Red);
  redSprite.move(-5, 0);

  greenSprite.setTexture(handTexture);
  greenSprite.setColor(sf::Color::Green);
  greenSprite.move(5, 0);

  blueSprite.setTexture(handTexture);
  blueSprite.setColor(sf::Color::Blue);
  blueSprite.move(0, 5);

  sf::RenderStates add = sf::RenderStates(sf::BlendAdd);
  window->clear(sf::Color::Black);
  window->draw(redSprite, add);
  window->draw(greenSprite, add);
  window->draw(blueSprite, add);
  window->display();
}
