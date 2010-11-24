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

#include "imageprocessor.h"
#include <iostream>
using namespace std;

ImageProcessor::ImageProcessor(int width, int height)
{
    images = 0;
    oldImage = new QImage(width,height,QImage::Format_RGB32);
    avgImage = new QImage(width,height,QImage::Format_RGB32);
}

QImage ImageProcessor::processImage(const QImage &image)
{
    QImage img(image);

    unsigned sum = 0;
    for(int x = 0;x<img.width();x++)
    {
        for(int y = 0;y<img.height();y++)
        {
            QColor c1(image.pixel(x,y)),c2(oldImage->pixel(x,y)),c3(avgImage->pixel(x,y));

            //greyscale difference
            //uint g = abs((4*c1.red()+3*c1.green()+3*c1.blue()) - (4*c2.red()+3*c2.green()+3*c2.blue()))/10;
            uint g = abs((4*c1.red()+3*c1.green()+3*c1.blue())/10 - (c3.blue()));


            //count pixels with difference> treshold
            if (g>TRESHOLD) sum++;
            else g = 0;

            {
                QColor c(g,g,g);
                img.setPixel(x,y,c.rgb());
            }

            //average image(assume avg image is greyscale)

            uint gsavg = (((4*c1.red()+3*c1.green()+3*c1.blue())/10)+(images)*c3.blue())/(images+1);





            {
                QColor c(gsavg,gsavg,gsavg);
                avgImage->setPixel(x,y,c.rgb());
            }

        }
    }
    //if (images<NUM_PICS)
        images++;

    //redscale image if big difference
    if(sum>img.width()*img.height()/RATIO)
    {
        for(int x = 0;x<img.width();x++)
        {
            for(int y = 0;y<img.height();y++)
            {
                img.setPixel(x,y,img.pixel(x,y)%256*256*256);
            }
        }
    }

    //delete avgImage;
    delete oldImage;
    oldImage = new QImage(image);

    return img;
    //return *avgImage;
    //return image;
}

ImageProcessor::~ImageProcessor()
{
    delete oldImage;
    delete avgImage;
}
