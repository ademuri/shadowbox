#include "Cartoon.hpp"
#include <SFML/Window.hpp>
#include <cstdlib>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

Cartoon::Cartoon(sf::RenderWindow *const window) : Effect(window) {
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
  std::cout << "Shader available: " << sf::Shader::isAvailable() << std::endl;
  if (!shader.loadFromFile("fragment_shader.frag", sf::Shader::Fragment)) {
    std::cout << "Unable to load shader" << std::endl;
  }
}

void Cartoon::render(cv::Mat &frame) {
  // Highlight the hand in white, and make the rest of output transparent.
  findHand(frame, handMask, backMask);
  output.setTo(Scalar(255, 255, 255, 255), backMask);
  output.setTo(Scalar(0, 0, 0, 0), handMask);

  Mat structuringElement = getStructuringElement(MORPH_RECT, Size(6, 6), Point(-1, -1));
  cv::erode(output, output, structuringElement, Point(-1, -1), 1 /* iterations */);
  cv::dilate(output, output, structuringElement, Point(-1, -1), 1 /* iterations */);

  sf::Image handImage;
  sf::Texture handTexture;
  sf::Sprite handSprite;

  createSfImage(output, handImage);
  handTexture.loadFromImage(handImage);
  handSprite.setTexture(handTexture);

  window->clear(sf::Color::Black);
  window->draw(handSprite, &shader);
  window->display();
}
