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

#include "colorimage.h"

Color(r, g, b):r(r),g(g),b(b)
{
}

Color(uint color)
{
  r = (color >> 16);
  g = ((color >> 8) & 0xff) ;
  b = (color & 0xff);
}

uint Color::toUintColor()
{
  return (0xFF000000 | b | (g << 8) | (r << 16));
}

uchar Color::toGrayScale()
{
  return (4*r+3*g+3*b)/10;
}

//---------------------------------------------------

ColorImage::ColorImage()
{
}

ColorImage::toUint32Color(Color c)
{
  return c.toUintColor();
}


GrayScaleImage ColorImage::toGrayScale()
{
    HCImage<uchar>::ImageBuffer b;
    b.resize(width()*height());
    for(int y = 0; y<height();y++)
    {
      for(int x = 0; x<width();x++)//todo optimize
      {
        b[x+y*width()]= pixel(x,y).toGrayScale();// imageData[sx+x+(sy+y)*w];
      }
    }
    HCImage<uchar> h(b,width(),height());
    return h;
}*/

bool ColorImage::similar(Color reference, Color color, uint treshold)
(uint reference,uint color, uint treshold)
{
  return abs(reference.r-color.r)<=treshold && abs(reference.g-color.g)<=treshold && abs(reference.b-color.b)<=treshold;
}

/*void ColorImage::saveImage(int index, string fname)
{
    if(fname==""){
        //zapis do suboru
        stringstream ssfname;
        ssfname << "hand_images/"<<  "full" << "_" << index << ".pbm";
        fname = ssfname.str();
    }

    ofstream ofs(fname.c_str());

    ofs << "P2" << endl;
    ofs << width() << " " << height() << endl;
    ofs << 256 << endl;
    for(int y = 0;y < height(); y++)
    {
      for(int x = 0;x < width(); x++)
      {
        ofs << (int)pixel(x,y) << " ";
      }
      ofs << endl;
    }
    ofs.close();
}*/

