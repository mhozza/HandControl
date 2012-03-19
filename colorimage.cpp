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


ColorImage::ColorImage():HCImage<Color>()
{
  ppmType = "P3";
}

ColorImage::ColorImage(unsigned w, unsigned h):HCImage<Color>(w,h)
{
  ppmType = "P3";
}

ColorImage::ColorImage(ImageBuffer img, unsigned w, unsigned h):HCImage<Color>(img,w,h)
{
  ppmType = "P3";
}

HCImage<Color> * ColorImage::create(ImageBuffer img, unsigned w, unsigned h)
{
  return new ColorImage(img,w,h);
}

uint ColorImage::toUint32Color(Color c)
{
  return c.toUintColor();
}


GrayScaleImage * ColorImage::toGrayScale()
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
    GrayScaleImage * h = new GrayScaleImage(b,width(),height());
    return h;
}

bool ColorImage::similar(Color reference, Color color, uint treshold)
{
  return abs(reference.red()-color.red())<=treshold && abs(reference.green()-color.green())<=treshold && abs(reference.blue()-color.blue())<=treshold;
}
/*
void ColorImage::saveImage(int index, string fname)
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
        ofs << pixel(x,y) << " ";
      }
      ofs << endl;
    }
    ofs.close();
}
*/
Color ColorImage::interpolatePixel(float x, float y)
{
  float dx = x-floor(x);
  float dy = y-floor(y);
  Color p1 = pixel(round(x),round(y))*(0.5+dx*0.5)+pixel(round(x)+1,round(y))*(0.5-dx*0.5);
  Color p2 = pixel(round(x),round(y)+1)*(0.5+dx*0.5)+pixel(round(x)+1,round(y)+1)*(0.5-dx*0.5);
  return (p1*(0.5+dy*0.5)+p2*(0.5-dy*0.5));
}

string ColorImage::color2String(Color color)
{
  return color.toString();
}


Color ColorImage::getAverageColor(int x, int y)
{
  int colorR = 0;
  int colorG = 0;
  int colorB = 0;
  for(int i = x-1;i<=x+1;i++)
  {
    for(int j = y-1;j<=y+1;j++)
    {
      colorR += pixel(i,j).red();
      colorG += pixel(i,j).red();
      colorB += pixel(i,j).red();
    }
  }
  return Color(round(colorR/9.0),round(colorG/9.0),round(colorB/9.0));
}
