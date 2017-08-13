#include "Projector.hpp"
#include "RgbUtil.hpp"
#include <cmath>
#include <cstdio>
#include <unistd.h>

// TODO: change this to false for production
const bool PROJECTOR_DEBUG = true;

void debugLog(const char *message) {
  if (PROJECTOR_DEBUG) {
    fprintf(stderr, message);
  }
}

Projector::Projector() {
  if (!fadecandy.resolve("127.0.0.1")) {

    debugLog("Fadecandy unable to resolve\n");
  }
  if (!fadecandy.tryConnect()) {
    debugLog("Fadecandy unable to connect\n");
  }

  int frameBytes = numPixels * 3;
  packet.resize(sizeof(OPCClient::Header) + frameBytes);
  OPCClient::Header::view(packet).init(0, fadecandy.SET_PIXEL_COLORS,
                                       frameBytes);
}

void Projector::setColor(RgbColor color) {
  stageEveryNColor(1, 0, color);
  show();
}

void Projector::setColor(unsigned char red, unsigned char green,
                         unsigned char blue) {
  stageEveryNColor(1, 0, red, green, blue);
  show();
}

void Projector::stageEveryNColor(unsigned int n, unsigned int offset,
                                 RgbColor color) {
  stageEveryNColor(n, offset, color.r, color.g, color.b);
}

void Projector::stageEveryNColor(unsigned int n, unsigned int offset,
                                 unsigned char red, unsigned char green,
                                 unsigned char blue) {
  uint8_t *dest = OPCClient::Header::view(packet).data();
  for (unsigned int i = offset; i < numPixels; i += n) {
    // Set brightness to half - otherwise the ring is blinding
    dest[i * 3] = red / 2;
    dest[i * 3 + 1] = green / 2;
    dest[i * 3 + 2] = blue / 2;
  }
}

void Projector::show() {
  if (!fadecandy.isConnected()) {
    if (!fadecandy.tryConnect()) {
      debugLog("Fadecandy unable to connect\n");
    }
  }
  if (!fadecandy.write(packet)) {
    debugLog("Fadecandy unable to write\n");
  }
}

uint8_t ramp(uint8_t val) {
  if (val < 100) {
    return 0;
  }
  return (val - 100) * 2;
}

void Projector::animateScreenOn() {
  setColor(0, 0, 0);
  setColor(0, 0, 0);

  setColor(255, 255, 255);
  setColor(255, 255, 255);
  usleep(1 * 1000 * 50);

  setColor(0, 0, 0);
  setColor(0, 0, 0);
  usleep(1 * 1000 * 50);

  // Fade in from RGB -> white, and then blink twice
  for (int i = 0; i < 200; i++) {
    stageEveryNColor(3, 0, i, ramp(i), ramp(i));
    stageEveryNColor(3, 1, ramp(i), i, ramp(i));
    stageEveryNColor(3, 2, ramp(i), ramp(i), i);
    show();
    usleep(1 * 1000 * 16 + 500);
  }
  setColor(0, 0, 0);
  usleep(1 * 1000 * 50);

  setColor(255, 255, 255);
  usleep(1 * 1000 * 50);

  setColor(0, 0, 0);
  usleep(1 * 1000 * 50);

  setColor(255, 255, 255);
  usleep(1 * 1000 * 50);

  setColor(0, 0, 0);
  usleep(1 * 1000 * 50);
}

void Projector::screenOffAnimationTick() {
  static HsvColor color = {0, 255, 255};
  static unsigned int frameCount = 0;
  static const unsigned int colorStep = 6;
  // When this is true, we're flashing brightly (for a short period)
  static bool flashMode = false;
  static unsigned int flashCount = 0;

  // Every so often, do a flash.
  if ((rand() % (60 * 60 * 5)) == 0) {
    flashCount = rand() % 10 + 10;
    flashMode = true;
  }

  if (flashMode) {
    color.v = (flashCount % 2 == 0) ? 255 : 0;

    // Every so often, change the color when flashing
    if (rand() % 100 == 0) {
      color.h = rand() % 256;
    }

    // The fadecandy interpolates between successive commands. Send two pretty
    // quickly to make it flash "harder"
    setColor(RgbUtil::toRgb(color));
    usleep(1000);
    setColor(RgbUtil::toRgb(color));

    flashCount--;
    if (flashCount == 0) {
      flashMode = false;
    }
    usleep(100 * 1000);
  } else {
    if (frameCount % colorStep == 0) {
      color.h++;
    }

    color.v = 45 + 209 * fabs(sin(0.005f * frameCount));

    // This displays four (24 / 6 = 4) "notches" that spin in the projector
    // light.
    int NUM_NOTCHES = 6;
    for (int i = 0; i < NUM_NOTCHES; i++) {
      HsvColor specificColor = color;
      specificColor.v =
          fmin(color.v, 20 + 244 * fabs(sin(.004f * frameCount + i * .5f)));
      stageEveryNColor(6, i, RgbUtil::toRgb(specificColor));
    }
    show();
  }

  frameCount++;
}
