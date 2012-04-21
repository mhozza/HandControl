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
/*
#define RED(c) (c >> 16)
#define GREEN(c) ((c >> 8) & 0xff)
#define BLUE(c) (c & 0xff)
#define RGB(r,g,b) (0xFF000000 | b | (g << 8) | (r << 16))
*/
using namespace std;

class Utils;

template <class T>
class HCImage
{    
public:
  typedef vector<T> ImageBuffer;
private:
  bool init;
protected:
  unsigned w, h;  
  string ppmType;
  ImageBuffer imageData;

  void construct(unsigned w, unsigned h);
  virtual bool similar(T reference,T color, uint treshold) = 0;
  virtual uint toUint32Color(T c) = 0;
  virtual string color2String(T color) = 0;
  virtual HCImage<T>* create(ImageBuffer img, unsigned w, unsigned h) = 0;
  virtual T getAverageColor(int x, int y) = 0;
  virtual T invertColor(T color) = 0;
public:
  HCImage():init(false),w(0),h(0),ppmType("P2") { }
    HCImage(unsigned w, unsigned h);
    HCImage(ImageBuffer img, unsigned w, unsigned h);
    ~HCImage();
    ImageBuffer image()
    {
        return imageData;
    }

    void setImage(ImageBuffer img, unsigned w, unsigned h);

    void mask(HCImage *mask, bool invert = false);
    unsigned width();
    unsigned height();
    T pixel(int x, int y);
    virtual T interpolatePixel(float x, float y) = 0;
    void setPixel(unsigned x, unsigned y, T val);
    HCImage<T>* getFloodFillSelectionMask(unsigned sx, unsigned sy, int treshold = 5);
    HCImage<T>* getAdaptiveFloodFillSelectionMask(unsigned sx, unsigned sy, int treshold = 5,  float originalFactor = 0.35, float changeFactor = 0.5);
    HCImage * copy(QRect r);
    void scale(unsigned w, unsigned h);
    inline bool isNull(){return !init;}
    QImage toQImage();    
    virtual void saveImage(int index = 0, string fname = "");
    virtual string saveImageToString();
    void invert();
};

template <class T>
unsigned HCImage<T>::width()
{
    return w;
}

template <class T>
unsigned HCImage<T>::height()
{
    return h;
}


template <class T>
void HCImage<T>::construct(unsigned w, unsigned h)
{
    this->w = w;
    this->h = h;
    init = true;
    ppmType = "P2";
}

template <class T>
HCImage<T>::HCImage(unsigned w, unsigned h)
{    
    construct(w,h);
    imageData.resize(w*h,0);
}

template <class T>
HCImage<T>::HCImage(ImageBuffer img, unsigned w, unsigned h)
{
    setImage(img,w,h);
}

template <class T>
HCImage<T>::~HCImage()
{    
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
    x = max(0,min(x,(int)w-1));
    y = max(0,min(y,(int)h-1));
    return imageData[x+y*w];
}

template <class T>
void HCImage<T>::setPixel(unsigned x, unsigned y, T val)
{
    x = min(x,w-1);
    y = min(y,h-1);
    imageData[x+y*w] = val;
}

template <class T>
QImage HCImage<T>::toQImage()
{    
    QImage img(w,h,QImage::Format_RGB888);

    for(unsigned y = 0; y<h;y++)
    {
        for(unsigned x = 0; x<w;x++)
        {
            T bc = pixel(x,y);
            uint c = toUint32Color(bc);
            img.setPixel(x,y,c);
        }
    }
    return img;
}

template <class T>
HCImage<T>* HCImage<T>::copy(QRect r)
{
    ImageBuffer b;
    unsigned width=r.width(), height = r.height(), sx = r.left(), sy = r.top();
    b.resize(width*height);
    for(unsigned y = 0; y<height;y++)
    {
        for(unsigned x = 0; x<width;x++)//todo optimize
        {
            b[x+y*width]=imageData[sx+x+(sy+y)*w];
        }
    }
    HCImage<T> *h = create(b,r.width(),r.height());
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
void HCImage<T>::mask(HCImage<T> *mask, bool invert)
{
    if(mask->height()!=h || mask->width()!=w) throw 1;

    for(unsigned y = 0; y<h;y++)
    {
        for(unsigned x = 0; x<w;x++)
        {
            if(!invert)
                setPixel(x,y,(pixel(x,y) & mask->pixel(x,y)));
            else
                setPixel(x,y,(pixel(x,y) | mask->pixel(x,y)));
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

    ofs << ppmType << endl;
    ofs << width() << " " << height() << endl;
    ofs << 256 << endl;
    for(unsigned y = 0;y < height(); y++)
    {
      for(unsigned x = 0;x < width(); x++)
      {
        ofs << color2String(pixel(x,y)) << " ";
      }
      ofs << endl;
    }
    ofs.close();
}

template <class T>
string HCImage<T>::saveImageToString()
{
    ostringstream ofs;

    ofs << ppmType << endl;
    ofs << width() << " " << height() << endl;
    ofs << 256 << endl;
    for(unsigned y = 0;y < height(); y++)
    {
      for(unsigned x = 0;x < width(); x++)
      {
        ofs << color2String(pixel(x,y)) << " ";
      }
      ofs << endl;
    }
    return ofs.str();
}

template <class T>
HCImage<T>* HCImage<T>::getFloodFillSelectionMask(unsigned sx, unsigned sy, int treshold)
{   
  return getAdaptiveFloodFillSelectionMask(sx,sy,treshold,1);
}

template <class T>
HCImage<T>* HCImage<T>::getAdaptiveFloodFillSelectionMask(unsigned sx, unsigned sy, int treshold, float originalFactor, float changeFactor)
{ 
  //T reference = pixel(sx,sy);
  T reference = getAverageColor(sx,sy);
  ImageBuffer b;
  b.resize(width()*height(),0);
  queue<pair<pair<unsigned,unsigned>,T> > f;
  f.push(make_pair(make_pair(sx,sy),reference));
  while(!f.empty())
  {
      unsigned x = f.front().first.first;
      unsigned y = f.front().first.second;
      T refcolor = f.front().second;
      f.pop();
      if(x>=width()) continue;
      if(y>=height()) continue;

      T color = pixel(x,y);
      if(!similar(refcolor,color,treshold) || b[x+y*w]!=T(0)) continue;
      b[x+y*w]=0xffffffff;
      refcolor = reference*originalFactor + (refcolor*changeFactor+color*(1-changeFactor))*(1-originalFactor);
      //refcolor = color;
      f.push(make_pair(make_pair(x+1,y),refcolor));
      f.push(make_pair(make_pair(x-1,y),refcolor));
      f.push(make_pair(make_pair(x,y+1),refcolor));
      f.push(make_pair(make_pair(x,y-1),refcolor));

  }
  HCImage<T> * maskImage = create(b,width(),height());
  return maskImage;
}

template <class T>
void HCImage<T>::invert()
{
    for(unsigned i = 0;i<width()*height();i++)
    {
        imageData[i] = invertColor(imageData[i]);
    }
}

#endif // HCIMAGE_H
