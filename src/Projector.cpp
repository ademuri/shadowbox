#include "Projector.hpp"
#include <cstdio>

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

void Projector::setColor(unsigned char red, unsigned char green,
                         unsigned char blue) {
  setEveryNColor(1, 0, red, green, blue);
}

void Projector::setEveryNColor(unsigned int n, unsigned int offset,
                               unsigned char red, unsigned char green,
                               unsigned char blue) {
  uint8_t *dest = OPCClient::Header::view(packet).data();
  for (unsigned int i = offset; i < numPixels; i += n) {
    dest[i * 3] = red;
    dest[i * 3 + 1] = green;
    dest[i * 3 + 2] = blue;
  }

  // Write twice because the fadecandy does automatic interpolation, so if we
  // don't, it'll slooowwwly fade to the color we specify.
  if (!fadecandy.write(packet)) {
    fprintf(stderr, "Fadecandy unable to write\n");
  }
  if (!fadecandy.write(packet)) {
    fprintf(stderr, "Fadecandy unable to write\n");
  }
}
