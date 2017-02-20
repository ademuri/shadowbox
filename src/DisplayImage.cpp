#include "BasicHighlight.hpp"
#include "BasicTracer.hpp"
#include "Effect.hpp"
#include "FlickerShadow.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cv.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int displaySdl() {
  VideoCapture cap;
  if (cap.open(0)) {
    std::cout << "Opened the camera." << std::endl;
  } else {
    std::cout << "Couldn't open the camera!" << std::endl;
    return 1;
  }

  if (!cap.set(CAP_PROP_FPS, 120)) {
    std::cout << "Couldn't set the FPS" << std::endl;
  }
  if (!cap.set(CAP_PROP_FRAME_WIDTH, 320)) {
    std::cout << "Couldn't set the width" << std::endl;
  }
  if (!cap.set(CAP_PROP_FRAME_HEIGHT, 240)) {
    std::cout << "Couldn't set the height" << std::endl;
  }
  if (!cap.set(CAP_PROP_AUTO_EXPOSURE, 0.25)) {
    std::cout << "couldn't set auto exposure" << std::endl;
  }
  // NOTE: this exposure value was chosen to make the hand light and the
  // background dark
  if (!cap.set(CAP_PROP_EXPOSURE, 0.005)) {
    std::cout << "couldn't set exposure" << std::endl;
  }

  // TODO: investigate capturing directly in grayscale
  /*if (!cap.set(CV_CAP_PROP_FORMAT, CV_8UC1)) {
    std::cout << "Couldn't set the format" << std::endl;
    }*/

  sf::RenderWindow window(sf::VideoMode(320, 240), "My window");
  window.setVerticalSyncEnabled(false);

  Mat image;
  Mat frameRGBA;
  sf::Image sfImage;
  sf::Texture texture;
  sf::Sprite sprite;

  Effect *const effect = new BasicTracer(&window);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::KeyPressed:
          if (event.key.code == sf::Keyboard::Escape) {
            window.close();
          }
          break;

        default:
          // No-op
          break;
      }
    }

    cap.read(image);
    effect->render(image);
    effect->calculateFramerate();
  }

  cap.release();
  return 0;
}

int main(int argc, char **argv) { return displaySdl(); }
