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

#include <vector>
#include <QImage>
#include <QMutex>
#include <fftw3.h>

using namespace std;

typedef unsigned char uchar;
typedef vector<uchar> ImageBuffer;

class HCImage
{    
    bool init;
    unsigned w, h;
    ImageBuffer imageData;
    //QMutex * mutex;
    void construct(unsigned w, unsigned h);
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

    uchar pixel(int x, int y);
    uchar interpolatePixel(float x, float y);

    HCImage getFullFillSelectionMask(int sx, int sy, int treshold = 5);

    void setPixel(unsigned x, unsigned y, uchar val);

    HCImage copy(QRect r);
    void scale(unsigned w, unsigned h);

    inline bool isNull(){return !init;}

    QImage toQImage();
    fftw_complex * toComplexArray();
    double * toDoubleArray();
};

#endif // HCIMAGE_H
