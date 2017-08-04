#include "Screen.hpp"
#include <cstdio>
#include <ftdi.h>

Screen::Screen() {
  if ((ftdic = ftdi_new()) == 0) {
    fprintf(stderr, "ftdi_new failed!\n");
  }

  int f = ftdi_usb_open(ftdic, 0x0403, 0x6001);
  if (f < 0 && f != -5) {
    fprintf(stderr, "unable to open ftdi device: %d (%s)\n", f,
            ftdi_get_error_string(ftdic));
  }

  if (ftdi_set_bitmode(ftdic, 0xFF, BITMODE_BITBANG) < 0) {
    fprintf(stderr, "unable to set bitbang mode: (%s)\n",
            ftdi_get_error_string(ftdic));
  }
}

void Screen::turnOff() {
  unsigned char cmd = 0x00;
  int written = ftdi_write_data(ftdic, &cmd, 1);
  if (written < 0) {
    fprintf(stderr, "unable to write data: %s\n", ftdi_get_error_string(ftdic));
  }
}

void Screen::turnOn() {
  unsigned char cmd = 0x08;
  int written = ftdi_write_data(ftdic, &cmd, 1);
  if (written < 0) {
    fprintf(stderr, "unable to write data: %s\n", ftdi_get_error_string(ftdic));
  }
}
