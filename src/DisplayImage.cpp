#define GL_GLEXT_PROTOTYPES

#include "BasicHighlight.hpp"
#include "BasicTracer.hpp"
#include "Cartoon.hpp"
#include "Effect.hpp"
#include "FlickerShadow.hpp"
#include "RgbSplit.hpp"
#include <GLES3/gl3.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
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

  sf::RenderWindow window(sf::VideoMode(320, 240), "My window", sf::Style::Default, sf::ContextSettings(0, 0, 0, 2, 0));

  sf::ContextSettings settings = window.getSettings();
  std::cout << "Version: " << settings.majorVersion << "." << settings.minorVersion << std::endl;

  std::string vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
  std::string renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
  std::cout << "Vendor: " << vendor << " Renderer: " << renderer << std::endl;
  std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Extensions: " << glGetString(GL_EXTENSIONS) << std::endl;

  Mat image;

  Effect *const effect = new Cartoon(&window);

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
