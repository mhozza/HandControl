/*
 *    HandControl - Hand gesture recognition
 *    Copyright (C) 2010  Michal Hozza (mhozza@gmail.com)
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

#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>
#include <QColor>
#include <QThread>
#include <QFuture>
#include <QtConcurrentRun>
#include <QMutex>

#include <queue>

#include "handrecognizer.h"

using namespace std;

#define TRESHOLD 20
#define RATIO 24
#define MAX_FRAMES 40
#define PIXEL_RADIUS 4
#define MIN_RECT_SIZE 40
#define MAX_RECT_SIZE 220

class ImageProcessor
{
    QImage *oldImage, img, *expandedImgX, *expandedImg;
    int images,images2;
    bool imgChanged;
    unsigned sum;
    QMutex imgLock;
    HandRecognizer * handRecognizer;
    queue<pair<QRect,uint> > rectQueue;

    template<class T> inline void exchange(T &a, T &b)
    {
      T t = a;
      a = b;
      b = t;
    }

    template<class T> inline T median3(T a, T b, T c)
    {
      if(b>a) exchange(a,b);
      if(c>a) exchange(a,c);
      if(c>b) exchange(b,c);
      return b;
    }

    void prepareImg(const QImage &, int  ,int  ,int ,int );    

    void medianFilterX(int sy, int ex, int ey, QImage * imgIn, QImage * imgOut);
    void medianFilterY(int sx, int ex, int ey, QImage * imgIn, QImage * imgOut);

    void expandPixelsX(int sy, int ex, int ey, QImage * imgIn, QImage * imgOut);
    void expandPixelsY(int sx, int ex, int ey, QImage * imgIn, QImage * imgOut);
    QRect segment(int x, int y, uint color, QImage * image, QRect rect);

public:
    ImageProcessor(int width, int height, HandRecognizer * handRecognizer);
    ~ImageProcessor();
    QImage processImage(const QImage &img);
    inline bool imageChanged(){return imgChanged;}
};

#endif // IMAGEPROCESSOR_H