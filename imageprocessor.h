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

#define TRESHOLD 20
#define RATIO 24
#define MAX_FRAMES 40


class ImageProcessor
{
    const QImage *oldImage;
    QImage *avgImage, *avgImage2;
    int images,images2;
    bool firstAvg, avgCmp, imgChanged;
public:
    ImageProcessor(int width, int height);
    ~ImageProcessor();
    QImage processImage(const QImage &img);
    inline QImage getAvgImage()
    {
        if(firstAvg)
            return *avgImage;
        else
            return *avgImage2;
    }
    inline void setAvgCmp(bool b){avgCmp = b;}
    inline bool imageChanged(){return imgChanged;}
};

#endif // IMAGEPROCESSOR_H
