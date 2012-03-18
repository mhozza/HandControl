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

ColorImage::ColorImage()
{
}

ColorImage::toUint32Color(Color c)
{
  return c.toUintColor();
}


GrayScaleImage ColorImage::toGrayScale()
{
    GrayScaleImage::ImageBuffer b;
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
}

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

uchar GrayScaleImage::interpolatePixel(float x, float y)
{
    float dx = x-floor(x);
    float dy = y-floor(y);
    float p1 = (0.5+dx*0.5)*pixel(round(x),round(y))+(0.5-dx*0.5)*pixel(round(x)+1,round(y));
    float p2 = (0.5+dx*0.5)*pixel(round(x),round(y)+1)+(0.5-dx*0.5)*pixel(round(x)+1,round(y)+1);
    return round(((0.5+dy*0.5)*p1+(0.5-dy*0.5)*p2));
}
