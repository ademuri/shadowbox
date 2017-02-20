#include "BasicTracer.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

BasicTracer::BasicTracer(sf::RenderWindow *const window) : Effect(window) {
  handImage.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
  accumulator.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
  output.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC4);
}

void BasicTracer::render(Mat &frame) {
  // Highlight the hand in red, and make the rest of output transparent.
  findHand(frame, handMask, backMask);
  handImage.setTo(Scalar(255, 0, 0, 255), backMask);
  handImage.setTo(Scalar(0, 0, 0, 0), handMask);

  addWeighted(handImage, 1.0, accumulator, 0.9, 0, output, -1);
  output.copyTo(accumulator);

  sf::Image image;
  sf::Texture texture;
  sf::Sprite sprite;

  createSfImage(output, image);
  texture.loadFromImage(image);
  sprite.setTexture(texture);

  window->draw(sprite);
  window->display();
}
