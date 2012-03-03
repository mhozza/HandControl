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

#include "hcimage.h"
#include <cstring>
#include <cmath>
#include "utils.h"

void HCImage::construct(unsigned w, unsigned h)
{
    this->w = w;
    this->h = h;
    init = true;
}

HCImage::HCImage(unsigned w, unsigned h)
{    
    //mutex = new QMutex();
    construct(w,h);
    imageData.resize(w*h,0);
}

HCImage::HCImage(ImageBuffer img, unsigned w, unsigned h)
{
    //mutex = new QMutex();
    setImage(img,w,h);
}

HCImage::~HCImage()
{
    //delete mutex;
}

void HCImage::setImage(ImageBuffer img, unsigned w, unsigned h)
{

   //ToDo: check w,h
   construct(w,h);
   imageData = img;

}


uchar HCImage::pixel(int x, int y)
{    
    //ToDo: check init
    x = max(0,min(x,(int)w-1));
    y = max(0,min(y,(int)h-1));
    return imageData[x+y*w];
}

uchar HCImage::interpolatePixel(float x, float y)
{
    //ToDo: check init
    float dx = x-floor(x);
    float dy = y-floor(y);
    float p1 = (0.5+dx*0.5)*pixel(round(x),round(y))+(0.5-dx*0.5)*pixel(round(x)+1,round(y));
    float p2 = (0.5+dx*0.5)*pixel(round(x),round(y)+1)+(0.5-dx*0.5)*pixel(round(x)+1,round(y)+1);
    return round(((0.5+dy*0.5)*p1+(0.5-dy*0.5)*p2));
}


void HCImage::setPixel(unsigned x, unsigned y, uchar val)
{    
    //ToDo: check init
    x = min(x,w-1);
    y = min(y,h-1);
    imageData[x+y*w] = val;    
}

QImage HCImage::toQImage()
{    
    //ToDo: check init
    QImage img(w,h,QImage::Format_RGB888);

    for(int y = 0; y<h;y++)
    {
        for(int x = 0; x<w;x++)
        {
            uint bc = pixel(x,y);
            uint c = 0xFF000000 | (bc) | (bc << 8) | (bc << 16);
            img.setPixel(x,y,c);
        }
    }    
    return img;
}

HCImage HCImage::copy(QRect r)
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
    HCImage h(b,r.width(),r.height());
    return h;
}

void HCImage::scale(unsigned w, unsigned h)
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

fftw_complex * HCImage::toComplexArray()
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

double * HCImage::toDoubleArray()
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

void HCImage::setImageFromComplexArray(fftw_complex *b , unsigned w, unsigned h)
{
    //ToDo: check w,h
    construct(w,h);
    imageData.resize(w*h,0);
    for(int y = 0; y<h;y++)
    {
        for(int x = 0; x<w;x++)
        {
            uchar val = min((long)round(10*log2(Utils::cabs(b[x+y*w]))),255L);
            setPixel(x,y,val);
        }
    }

}

void HCImage::mask(HCImage mask, bool invert = false)
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


