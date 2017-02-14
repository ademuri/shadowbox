#ifndef __EFFECT_HPP__
#define __EFFECT_HPP__

#include <SDL2/SDL.h>
#include <cv.h>

/** A renderer that takes an image and outputs to the display.
 * The main loop will call #render for each frame retrieved from the camera.
 * This class is responsible for writing to the provided renderer.
 */
class Effect {
public:
  Effect(SDL_Renderer *const renderer_);

  /** Render one frame.
   * Called each time a new frame is retreived from the camera.
   */
  virtual void render(const cv::Mat &frame) = 0;

protected:
  SDL_Renderer *const renderer;

  /** Find the hand and put the hand and background masks into the provided
   * Mats.
   */
  void findHand(const cv::Mat frame, cv::Mat &handMask, cv::Mat &backMask);
  cv::Mat imageGray;

private:
};

#endif
