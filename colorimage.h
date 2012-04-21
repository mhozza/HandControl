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

#ifndef COLORIMAGE_H
#define COLORIMAGE_H

#include "color.h"
#include "hcimage.h"
#include "grayscaleimage.h"

class ColorImage : public HCImage<Color>
{
  bool similar(Color reference, Color color, uint treshold);
  uint toUint32Color(Color c);
  string color2String(Color color);
  HCImage<Color> * create(ImageBuffer img, unsigned w, unsigned h);
  Color getAverageColor(int x, int y);
  Color invertColor(Color color);
public:
  ColorImage();
  ColorImage(unsigned w, unsigned h);
  ColorImage(ImageBuffer img, unsigned w, unsigned h);
  Color interpolatePixel(float x, float y);

  GrayScaleImage * toGrayScale();
  //void saveImage(int index, string fname);
};

#endif // COLORIMAGE_H
