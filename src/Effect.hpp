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

  /** Renders one frame.
   * Called each time a new frame is retreived from the camera.
   */
  virtual void render(cv::Mat &frame) = 0;

protected:
  cv::Mat imageGray;
  SDL_Renderer *const renderer;

  /** Finds the hand and puts the hand and background masks into the provided
   * Mats.
   */
  void findHand(const cv::Mat frame, cv::Mat &handMask, cv::Mat &backMask);

  /** Creates an RGB SDL surface for the given OpenCV Mat.
   * Uses the BGR pixel ordering.
   */
  SDL_Surface *createRGBSurface(cv::Mat &frame) const;

  /** Creates an RGBA SDL surface for the given OpenCV Mat.
   * Uses the RGBA pixel ordering.
   */
  SDL_Surface *createRGBASurface(cv::Mat &frame) const;

  /** Creates an RGB texture for the given OpenCV Mat. */
  SDL_Texture *createRGBTexture(cv::Mat &frame);

  /** Creates an RGBA texture for the given OpenCV Mat. */
  SDL_Texture *createRGBATexture(cv::Mat &frame);

  static const int IMAGE_WIDTH = 320;
  static const int IMAGE_HEIGHT = 240;

private:
};

#endif
