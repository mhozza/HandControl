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

#include "grayscaleimage.h"

GrayScaleImage::GrayScaleImage()
{
}

uint GrayScaleImage::toUint32Color(uchar c)
{
  return c;
}

uchar GrayScaleImage::interpolatePixel(float x, float y)
{
    float dx = x-floor(x);
    float dy = y-floor(y);
    float p1 = (0.5+dx*0.5)*pixel(round(x),round(y))+(0.5-dx*0.5)*pixel(round(x)+1,round(y));
    float p2 = (0.5+dx*0.5)*pixel(round(x),round(y)+1)+(0.5-dx*0.5)*pixel(round(x)+1,round(y)+1);
    return round(((0.5+dy*0.5)*p1+(0.5-dy*0.5)*p2));
}

fftw_complex * GrayScaleImage::toComplexArray()
{
    fftw_complex * b = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * w * h);
    for(int y = 0; y<h;y++)
    {
        for(int x = 0; x<w;x++)
        {
            b[x+y*w][0] = pixel(x,y);
            b[x+y*w][1] = 0;
        }
    }
    return b;
}

double * GrayScaleImage::toDoubleArray()
{
    double * b = (double*) fftw_malloc(sizeof(double) * w * h);
    for(int y = 0; y<h;y++)
    {
        for(int x = 0; x<w;x++)
        {
            b[x+y*w] = pixel(x,y);
        }
    }
    return b;
}

void GrayScaleImage::setImageFromComplexArray(fftw_complex *b , unsigned w, unsigned h)
{
    //ToDo: check w,h
    construct(w,h);
    imageData.resize(w*h,0);
    for(int y = 0; y<h;y++)
    {
        for(int x = 0; x<w;x++)
        {
            T val = min((long)round(10*log2(Utils::cabs(b[x+y*w]))),255L);
            setPixel(x,y,val);
        }
    }

}

bool GrayScaleImage::similar(uchar reference,uchar color, uchar treshold)
{
  return abs(reference-color)<=treshold;
}

/*
template <class T>
T HCImage<T>::getAverageColor(int x, int y)
{
  T color = 0;
  for(int i = x-1;i<=x+1;i++)
  {
    for(int j = y-1;j<=y+1;j++)
    {
      T p = pixel(i,j);
      color = add(color,RGB(RED(p)/9,GREEN(p)/9,BLUE(p)/9));
    }
  }
}
*/
