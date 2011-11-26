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


uchar HCImage::pixel(unsigned x, unsigned y)
{    
    //ToDo: check init
    x = min(x,w-1);
    y = min(y,h-1);
    return imageData[x+y*w];
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
            unsigned oldX = round(x/xScaleFactor);
            unsigned oldY = round(y/yScaleFactor);
            b[x+y*w] = pixel(oldX,oldY);
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
            uchar val = min((long long)round(log(Utils::cabs(b[x+y*w]))),255LL);
            setPixel(x,y,val);
        }
    }

}
