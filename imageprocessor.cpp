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



ImageProcessor::ImageProcessor(int width, int height): images(0), images2(MAX_FRAMES/2), firstAvg(true), avgCmp(false)
{    
    oldImage = new QImage(width,height,QImage::Format_RGB32);
    avgImage = new QImage(width,height,QImage::Format_RGB32);
    avgImage2 = NULL;
}

QImage ImageProcessor::processImage(const QImage &image)
{
    QImage img(image);
    bool firstRender = false;
    if(avgImage2==NULL)
    {
        avgImage2 = new QImage(*avgImage);
        firstRender = true;
    }

    unsigned sum = 0;
    for(int x = 0;x<img.width();x++)
    {
        for(int y = 0;y<img.height();y++)
        {
            QColor c1(image.pixel(x,y)),c2(oldImage->pixel(x,y)),c3(avgImage->pixel(x,y)),c4(avgImage2->pixel(x,y));

            uint gsc1 = (4*c1.red()+3*c1.green()+3*c1.blue())/10;
            uint gsc2 = (4*c2.red()+3*c2.green()+3*c2.blue())/10;
            uint gsca = firstAvg ? c3.blue() : c4.blue();

            //greyscale difference                        
            uint g = abs(gsc1 - gsc2);
            if(avgCmp) g = abs(gsc1 - gsca);


            //count pixels with difference> treshold
            if (g>TRESHOLD) sum++;
            else g = 0;
            {
                QColor c(g,g,g);
                img.setPixel(x,y,c.rgb());
            }

            //average image(assume avg image is greyscale)
            uint gsavg = 0, gsavg2 = 0;
            if(images<MAX_FRAMES || images2)
            {
                gsavg = (gsc1+images*c3.blue())/(images+1);
                gsavg2 = (gsc1+images2*c4.blue())/(images2+1);

            }
            else
            {
                if(firstAvg)
                {
                    gsavg = gsc1;

                }
                else
                {
                    gsavg2 = gsc1;
                }
            }

            /*if(gsavg==0)
                cout << firstAvg << " " << images << endl;*/
            if(firstRender) gsavg2 = gsc1;

            QColor cavg(gsavg,gsavg,gsavg), cavg2(gsavg2,gsavg2,gsavg2);
            avgImage->setPixel(x,y,cavg.rgb());
            avgImage2->setPixel(x,y,cavg2.rgb());

        }
    }
    if(images==MAX_FRAMES || images2==MAX_FRAMES)
    {
        if(firstAvg)
        {
            qDebug("Reset 1");
            images = 0;
        }
        else
        {
            qDebug("Reset 2");
            images2 = 0;
        }
        firstAvg = !firstAvg;

    }
    //if (images<NUM_PICS)
        images++;
        images2++;
    imgChanged = false;

    //big difference
    if(sum>img.width()*img.height()/RATIO)
    {            
        imgChanged = true;
        //redscale image
       /* for(int x = 0;x<img.width();x++)
        {
            for(int y = 0;y<img.height();y++)
            {
                img.setPixel(x,y,img.pixel(x,y)%256*256*256);
            }
        }*/
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
    delete avgImage2;
}
