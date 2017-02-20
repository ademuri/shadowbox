#ifndef __EFFECT_HPP__
#define __EFFECT_HPP__

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cv.h>

/** A renderer that takes an image and outputs to the display.
 * The main loop will call #render for each frame retrieved from the camera.
 * This class is responsible for writing to the provided renderer.
 */
class Effect {
public:
  Effect(sf::RenderWindow *const window);

  /** Renders one frame.
   * Called each time a new frame is retreived from the camera.
   */
  virtual void render(cv::Mat &frame) = 0;

  /** Prints the current framerate, in FPS, to the console.
   * Note that this is approximate - every ~1000ms, it counts the frames. It
   * doesn't divide by the actual elapsed time.
   */
  void calculateFramerate();

protected:
  cv::Mat imageGray;
  sf::Clock clock;
  sf::RenderWindow *const window;

  /** Finds the hand and puts the hand and background masks into the provided
   * Mats.
   */
  void findHand(const cv::Mat frame, cv::Mat &handMask, cv::Mat &backMask);

  /** Converts the given Mat to an SFML image. */
  void createSfImage(cv::Mat frame, sf::Image &image);

  static const int IMAGE_WIDTH = 320;
  static const int IMAGE_HEIGHT = 240;

private:
  // The number of frames in the past second
  int frames;
};

#endif
