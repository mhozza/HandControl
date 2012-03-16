/*
 *    HandControl - Hand gesture recognition 
 *    Version: 0.1
 * 
 *    Copyright (C) 2010-2011  Michal Hozza (mhozza@gmail.com)
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

#ifndef HCIMAGE_H
#define HCIMAGE_H

#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <cmath>
#include <queue>

#include <QImage>
#include <QMutex>

#include <fftw3.h>
#include "utils.h"

#define RED(c) (c >> 16)
#define GREEN(c) ((c >> 8) & 0xff)
#define BLUE(c) (c & 0xff)
#define RGB(r,g,b) (0xFF000000 | b | (g << 8) | (r << 16))
using namespace std;

class Utils;

template <class T>
class HCImage
{    
public:
  typedef vector<T> ImageBuffer;
private:
  bool init;
  unsigned w, h;
  ImageBuffer imageData;
  //QMutex * mutex;
  void construct(unsigned w, unsigned h);
  bool similar(uchar reference,uchar color, uchar treshold);
  bool similar(uint reference,uint color, uint treshold);
  uint toColor(uchar c) {return RGB(c,c,c);}
  uint toColor(uint c){return c;}
  uchar _toGrayScale(uchar c) {return c;}
  uchar _toGrayScale(uint c){return (4*RED(c)+3*GREEN(c)+3*BLUE(c))/10;}
  //T getAverageColor(int x, int y);
  /*uint addColor(uint c1,uint c2);
  uchar addColor(uchar c1,uchar c2);*/
public:    

    HCImage():init(false)
    {
    //    mutex = new QMutex();
    }
    HCImage(unsigned w, unsigned h);
    HCImage(ImageBuffer img, unsigned w, unsigned h);
    ~HCImage();
    inline ImageBuffer image()
    {
        //ToDo: check init
        return imageData;
    }
    void setImage(ImageBuffer img, unsigned w, unsigned h);
    void setImageFromComplexArray(fftw_complex *, unsigned w, unsigned h);    
    void mask(HCImage mask, bool invert = false);

    inline unsigned width()
    {
        //ToDo: check init
        return w;
    }
    inline unsigned height()
    {
        //ToDo: check init
        return h;
    }

    T pixel(int x, int y);
    T interpolatePixel(float x, float y);
    void setPixel(unsigned x, unsigned y, T val);
    HCImage<T> getFloodFillSelectionMask(int sx, int sy, int treshold = 5);

    HCImage copy(QRect r);
    void scale(unsigned w, unsigned h);

    inline bool isNull(){return !init;}

    QImage toQImage();
    fftw_complex * toComplexArray();
    double * toDoubleArray();
    void saveImage(int index = 0, string fname = "");
    HCImage<uchar> toGrayScale();

};

template <class T>
void HCImage<T>::construct(unsigned w, unsigned h)
{
    this->w = w;
    this->h = h;
    init = true;
}

template <class T>
HCImage<T>::HCImage(unsigned w, unsigned h)
{
    //mutex = new QMutex();
    construct(w,h);
    imageData.resize(w*h,0);
}

template <class T>
HCImage<T>::HCImage(ImageBuffer img, unsigned w, unsigned h)
{
    //mutex = new QMutex();
    setImage(img,w,h);
}

template <class T>
HCImage<T>::~HCImage()
{
    //delete mutex;
}

template <class T>
void HCImage<T>::setImage(ImageBuffer img, unsigned w, unsigned h)
{

   //ToDo: check w,h
   construct(w,h);
   imageData = img;

}

template <class T>
T HCImage<T>::pixel(int x, int y)
{
    //ToDo: check init
    x = max(0,min(x,(int)w-1));
    y = max(0,min(y,(int)h-1));
    return imageData[x+y*w];
}

template <class T>
T HCImage<T>::interpolatePixel(float x, float y)
{
    //ToDo: check init
    float dx = x-floor(x);
    float dy = y-floor(y);
    float p1 = (0.5+dx*0.5)*pixel(round(x),round(y))+(0.5-dx*0.5)*pixel(round(x)+1,round(y));
    float p2 = (0.5+dx*0.5)*pixel(round(x),round(y)+1)+(0.5-dx*0.5)*pixel(round(x)+1,round(y)+1);
    return round(((0.5+dy*0.5)*p1+(0.5-dy*0.5)*p2));
}

template <class T>
void HCImage<T>::setPixel(unsigned x, unsigned y, T val)
{
    //ToDo: check init
    x = min(x,w-1);
    y = min(y,h-1);
    imageData[x+y*w] = val;
}

template <class T>
QImage HCImage<T>::toQImage()
{
    //ToDo: check init
    QImage img(w,h,QImage::Format_RGB888);

    for(int y = 0; y<h;y++)
    {
        for(int x = 0; x<w;x++)
        {
            T bc = pixel(x,y);
            uint c = toColor(bc);
            img.setPixel(x,y,c);
        }
    }
    return img;
}

template <class T>
HCImage<T> HCImage<T>::copy(QRect r)
{
    ImageBuffer b;
    unsigned width=r.width(), height = r.height(), sx = r.left(), sy = r.top();
    b.resize(width*height);
    for(int y = 0; y<height;y++)
    {
        for(int x = 0; x<width;x++)//todo optimize
        {
            b[x+y*width]=imageData[sx+x+(sy+y)*w];
        }
    }
    HCImage<T> h(b,r.width(),r.height());
    return h;
}

template <class T>
void HCImage<T>::scale(unsigned w, unsigned h)
{
    //todo hodit chybu ak su 0 rozmery
    float xScaleFactor = (float)w/this->w;
    float yScaleFactor = (float)h/this->h;
    ImageBuffer b;
    b.resize(w*h);
    for(unsigned y = 0;y<h;y++)
    {
        for(unsigned x = 0;x<w;x++)
        {
            float oldX = x/xScaleFactor;
            float oldY = y/yScaleFactor;
            b[x+y*w] = interpolatePixel(oldX,oldY);
        }
    }
    setImage(b,w,h);
}

template <class T>
fftw_complex * HCImage<T>::toComplexArray()
{
    fftw_complex * b = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * w * h);
    //ToDo: check init
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

template <class T>
double * HCImage<T>::toDoubleArray()
{
    double * b = (double*) fftw_malloc(sizeof(double) * w * h);
    //ToDo: check init
    for(int y = 0; y<h;y++)
    {
        for(int x = 0; x<w;x++)
        {
            b[x+y*w] = pixel(x,y);
        }
    }
    return b;
}

template <class T>
void HCImage<T>::setImageFromComplexArray(fftw_complex *b , unsigned w, unsigned h)
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

template <class T>
void HCImage<T>::mask(HCImage<T> mask, bool invert)
{
    if(mask.height()!=h || mask.width()!=w) throw 1;

    for(int y = 0; y<h;y++)
    {
        for(int x = 0; x<w;x++)
        {
            if(!invert)
                setPixel(x,y,(pixel(x,y) & mask.pixel(x,y)));
            else
                setPixel(x,y,(pixel(x,y) | mask.pixel(x,y)));
        }
    }
}

template <class T>
void HCImage<T>::saveImage(int index, string fname)
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
}

template <class T>
HCImage<T> HCImage<T>::getFloodFillSelectionMask(int sx, int sy, int treshold)
{
  T reference = pixel(sx,sy);
  //T reference = getAverageColor(sx,sy);
  ImageBuffer b;
  b.resize(width()*height(),0);
  queue<pair<int,int> > f;
  f.push(make_pair(sx,sy));
  while(!f.empty())
  {
      int x = f.front().first;
      int y = f.front().second;
      //cout << x << " " << y << endl;
      f.pop();
      if(x<0 || x>=width()) continue;
      if(y<0 || y>=height()) continue;
      if(!similar(reference,pixel(x,y),treshold) || b[x+y*w]!=0) continue;
      b[x+y*w]=0xffffffff;
      f.push(make_pair(x+1,y));
      f.push(make_pair(x-1,y));
      f.push(make_pair(x,y+1));
      f.push(make_pair(x,y-1));

  }
  HCImage<T> maskImage(b,width(),height());
  return maskImage;
}

template <class T>
bool HCImage<T>::similar(uchar reference,uchar color, uchar treshold)
{
  return abs(reference-color)<=treshold;
}

template <class T>
bool HCImage<T>::similar(uint reference,uint color, uint treshold)
{
  int r = RED(color);
  int g = GREEN(color);
  int b = BLUE(color);

  int rr = RED(reference);
  int rg = GREEN(reference);
  int rb = BLUE(reference);

  return abs(rr-r)<=treshold && abs(rg-g)<=treshold && abs(rb-b)<=treshold;
}
/*
template <class T>
uchar HCImage<T>::addColor(uchar c1, uchar c2)
{
  return c1+c2;
}

template <class T>
uint HCImage<T>::addColor(uint c1, uint c2)
{
  int r = RED(c1);
  int g = GREEN(c1);
  int b = BLUE(c1);

  r += RED(c2);
  g += GREEN(c2);
  b += BLUE(c2);

  return RGB(r,g,b);
}
*/

template <class T>
HCImage<uchar> HCImage<T>::toGrayScale()
{
    HCImage<uchar>::ImageBuffer b;
    b.resize(width()*height());
    for(int y = 0; y<height();y++)
    {
      for(int x = 0; x<width();x++)//todo optimize
      {
        b[x+y*width()]= _toGrayScale(pixel(x,y));// imageData[sx+x+(sy+y)*w];
      }
    }
    HCImage<uchar> h(b,width(),height());
    return h;
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

#endif // HCIMAGE_H
