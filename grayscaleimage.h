/*
 *    Copyright (C) 2012  Michal Hozza (mhozza@gmail.com)
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GRAYSCALEIMAGE_H
#define GRAYSCALEIMAGE_H

#include "hcimage.h"
#include <sstream>

class GrayScaleImage : public HCImage<uchar>
{
  uint toUint32Color(uchar c);
  bool similar(uchar reference,uchar color, uint treshold);
  string color2String(uchar color);
  HCImage<uchar> * create(ImageBuffer img, unsigned w, unsigned h);
  uchar getAverageColor(int x, int y);
  uchar invertColor(uchar color);
public:
  GrayScaleImage();
  GrayScaleImage(unsigned w, unsigned h);
  GrayScaleImage(ImageBuffer img, unsigned w, unsigned h);
  //GrayScaleImage(GrayScaleImage& img);
  uchar interpolatePixel(float x, float y);

  fftw_complex * toComplexArray();
  double * toDoubleArray();
  void setImageFromComplexArray(fftw_complex *b , unsigned w, unsigned h);
  void loadFromPPM(string filename);
};

#endif // GRAYSCALEIMAGE_H
