#ifndef __RGB_UTIL_H__
#define __RGB_UTIL_H__

typedef struct RgbColor {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} RgbColor;

typedef struct HsvColor {
  unsigned char h;
  unsigned char s;
  unsigned char v;
} HsvColor;

class RgbUtil {
public:
  static RgbColor HsvToRgb(HsvColor hsv);
  static HsvColor RgbToHsv(RgbColor rgb);
};

#endif
