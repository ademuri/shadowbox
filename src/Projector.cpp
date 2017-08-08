#include "Projector.hpp"
#include "RgbUtil.hpp"
#include <cmath>
#include <cstdio>
#include <unistd.h>

Projector::Projector() {
  if (!fadecandy.resolve("127.0.0.1")) {
    fprintf(stderr, "Fadecandy unable to resolve\n");
  }
  if (!fadecandy.tryConnect()) {
    fprintf(stderr, "Fadecandy unable to connect\n");
  }

  int frameBytes = numPixels * 3;
  packet.resize(sizeof(OPCClient::Header) + frameBytes);
  OPCClient::Header::view(packet).init(0, fadecandy.SET_PIXEL_COLORS,
                                       frameBytes);
}

void Projector::setColor(RgbColor color) { setEveryNColor(1, 0, color); }

void Projector::setColor(unsigned char red, unsigned char green,
                         unsigned char blue) {
  setEveryNColor(1, 0, red, green, blue);
}

void Projector::setEveryNColor(unsigned int n, unsigned int offset,
                               RgbColor color) {
  setEveryNColor(n, offset, color.r, color.g, color.b);
}

void Projector::setEveryNColor(unsigned int n, unsigned int offset,
                               unsigned char red, unsigned char green,
                               unsigned char blue) {
  uint8_t *dest = OPCClient::Header::view(packet).data();
  for (unsigned int i = offset; i < numPixels; i += n) {
    // Set brightness to half - otherwise the ring is blinding
    dest[i * 3] = red / 2;
    dest[i * 3 + 1] = green / 2;
    dest[i * 3 + 2] = blue / 2;
  }

  if (!fadecandy.write(packet)) {
    fprintf(stderr, "Fadecandy unable to write\n");
  }
}

uint8_t ramp(uint8_t val) {
  if (val < 100) {
    return 0;
  }
  return (val - 100) * 2;
}

void Projector::animateScreenOn() {
  // Fade in from RGB -> white, and then blink twice
  for (int i = 0; i < 200; i++) {
    setEveryNColor(3, 0, i, ramp(i), ramp(i));
    setEveryNColor(3, 1, ramp(i), i, ramp(i));
    setEveryNColor(3, 2, ramp(i), ramp(i), i);
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

  if (frameCount % colorStep == 0) {
    color.h++;
  }

  color.v = 25 + 229 * fabs(sin(0.005f * frameCount));

  setColor(RgbUtil::toRgb(color));

  frameCount++;
}
