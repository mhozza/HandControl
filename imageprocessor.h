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


#define TRESHOLD 17
#define RATIO 24
#define MAX_FRAMES 40
#define PIXEL_RADIUS 3


class ImageProcessor
{
    QImage *oldImage, img, *expandedImgX, *expandedImg;
    int images,images2;
    bool imgChanged;
    unsigned sum;
    QMutex imgLock;
    void prepareImg(const QImage &, int  ,int  ,int ,int );    
    void expandPixelsX(int sy, int ex, int ey, QImage * imgIn, QImage * imgOut);
    void expandPixelsY(int sx, int ex, int ey, QImage * imgIn, QImage * imgOut);
public:
    ImageProcessor(int width, int height);
    ~ImageProcessor();
    QImage processImage(const QImage &img);    
    inline bool imageChanged(){return imgChanged;}
};

#endif // IMAGEPROCESSOR_H
