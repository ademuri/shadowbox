#include "RgbUtil.hpp"
#include <cstdlib>
#include <iostream>
#include <cmath>

using namespace std;

// Source: https://gist.github.com/fairlight1337/4935ae72bcbcc1ba5c72
// Copyright (c) 2014, Jan Winkler <winkler@cs.uni-bremen.de>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Universität Bremen nor the names of its
//       contributors may be used to endorse or promote products derived from
//       this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

/* Author: Jan Winkler */

// Lifted from https://stackoverflow.com/a/14733008
/*RgbColor RgbUtil::toRgb(const HsvColor hsv) {
  RgbColor rgb;
  unsigned char region, remainder, p, q, t;

  if (hsv.s == 0) {
    rgb.r = hsv.v;
    rgb.g = hsv.v;
    rgb.b = hsv.v;
    return rgb;
  }

  region = hsv.h / 43;
  remainder = (hsv.h - (region * 43)) * 6;

  p = (hsv.v * (255 - hsv.s)) >> 8;
  q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
  t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

  switch (region) {
  case 0:
    rgb.r = hsv.v;
    rgb.g = t;
    rgb.b = p;
    break;
  case 1:
    rgb.r = q;
    rgb.g = hsv.v;
    rgb.b = p;
    break;
  case 2:
    rgb.r = p;
    rgb.g = hsv.v;
    rgb.b = t;
    break;
  case 3:
    rgb.r = p;
    rgb.g = q;
    rgb.b = hsv.v;
    break;
  case 4:
    rgb.r = t;
    rgb.g = p;
    rgb.b = hsv.v;
    break;
  default:
    rgb.r = hsv.v;
    rgb.g = p;
    rgb.b = q;
    break;
  }

  return rgb;
}*/

HsvColor RgbUtil::toHsv(const RgbColor rgb) {
  HsvColor hsv;
  unsigned char rgbMin, rgbMax;

  rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b)
                         : (rgb.g < rgb.b ? rgb.g : rgb.b);
  rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b)
                         : (rgb.g > rgb.b ? rgb.g : rgb.b);

  hsv.v = rgbMax;
  if (hsv.v == 0) {
    hsv.h = 0;
    hsv.s = 0;
    return hsv;
  }

  hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
  if (hsv.s == 0) {
    hsv.h = 0;
    return hsv;
  }

  if (rgbMax == rgb.r)
    hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
  else if (rgbMax == rgb.g)
    hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
  else
    hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

  return hsv;
}

RgbColor RgbUtil::randomColor() {
  HsvColor color;
  color.h = rand() % 256;
  color.s = 255;
  color.v = 255;
  return toRgb(color);
}








/*! \brief Convert RGB to HSV color space
  
  Converts a given set of RGB values `r', `g', `b' into HSV
  coordinates. The input RGB values are in the range [0, 1], and the
  output HSV values are in the ranges h = [0, 360], and s, v = [0,
  1], respectively.
  
  \param fR Red component, used as input, range: [0, 1]
  \param fG Green component, used as input, range: [0, 1]
  \param fB Blue component, used as input, range: [0, 1]
  \param fH Hue component, used as output, range: [0, 360]
  \param fS Hue component, used as output, range: [0, 1]
  \param fV Hue component, used as output, range: [0, 1]
  
*/
/*void RGBtoHSV(float& fR, float& fG, float fB, float& fH, float& fS, float& fV) {
  float fCMax = max(max(fR, fG), fB);
  float fCMin = min(min(fR, fG), fB);
  float fDelta = fCMax - fCMin;
  
  if(fDelta > 0) {
    if(fCMax == fR) {
      fH = 60 * (fmod(((fG - fB) / fDelta), 6));
    } else if(fCMax == fG) {
      fH = 60 * (((fB - fR) / fDelta) + 2);
    } else if(fCMax == fB) {
      fH = 60 * (((fR - fG) / fDelta) + 4);
    }
    
    if(fCMax > 0) {
      fS = fDelta / fCMax;
    } else {
      fS = 0;
    }
    
    fV = fCMax;
  } else {
    fH = 0;
    fS = 0;
    fV = fCMax;
  }
  
  if(fH < 0) {
    fH = 360 + fH;
  }
}*/


/*! \brief Convert HSV to RGB color space
  
  Converts a given set of HSV values `h', `s', `v' into RGB
  coordinates. The output RGB values are in the range [0, 1], and
  the input HSV values are in the ranges h = [0, 360], and s, v =
  [0, 1], respectively.
  
  \param fR Red component, used as output, range: [0, 1]
  \param fG Green component, used as output, range: [0, 1]
  \param fB Blue component, used as output, range: [0, 1]
  \param fH Hue component, used as input, range: [0, 360]
  \param fS Hue component, used as input, range: [0, 1]
  \param fV Hue component, used as input, range: [0, 1]
  
*/
RgbColor RgbUtil::toRgb(const HsvColor hsv) {
  const float fH = hsv.h / 255.f * 360.f;
  const float fS = hsv.s / 255.f;
  const float fV = hsv.v / 255.f;
  float fR = 0;
  float fG = 0;
  float fB = 0;

  float fC = fV * fS; // Chroma
  float fHPrime = fmod(fH / 60.0, 6);
  float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
  float fM = fV - fC;
  
  if(0 <= fHPrime && fHPrime < 1) {
    fR = fC;
    fG = fX;
    fB = 0;
  } else if(1 <= fHPrime && fHPrime < 2) {
    fR = fX;
    fG = fC;
    fB = 0;
  } else if(2 <= fHPrime && fHPrime < 3) {
    fR = 0;
    fG = fC;
    fB = fX;
  } else if(3 <= fHPrime && fHPrime < 4) {
    fR = 0;
    fG = fX;
    fB = fC;
  } else if(4 <= fHPrime && fHPrime < 5) {
    fR = fX;
    fG = 0;
    fB = fC;
  } else if(5 <= fHPrime && fHPrime < 6) {
    fR = fC;
    fG = 0;
    fB = fX;
  } else {
    fR = 0;
    fG = 0;
    fB = 0;
  }
  
  fR += fM;
  fG += fM;
  fB += fM;

  RgbColor c;
  c.r = fR * 255;
  c.g = fG * 255;
  c.b = fB * 255;
  fprintf(stdout, "%3d %3d %3d\n", c.r, c.g, c.g);
  return c;
}
