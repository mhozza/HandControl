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

void ImageProcessor::expandPixelsX(int sy, int ex, int ey, QImage * imgIn, QImage * imgOut)
{
  int sx = 0;
  for(int y = sy;y<ey;y++)
  {
    int endblack = 0;
    for(int x = sx;x<sx+PIXEL_RADIUS && x<ex;x++)
    {
      QColor c(imgIn->pixel(x,y));
      if(c!=Qt::black)
      {
        endblack = x+PIXEL_RADIUS;
      }
    }
    for(int x = sx;x<ex;x++)
    {
      int xx = x+PIXEL_RADIUS;
      if(xx<ex)
      {
        QColor c(imgIn->pixel(xx,y));
        if(c!=Qt::black)
        {
          endblack = xx+PIXEL_RADIUS;
        }
      }
      if(x<endblack)
      {
         imgLock.lock();
         imgOut->setPixel(x,y,0xFF000000);
         imgLock.unlock();
      }
      else
      {
         imgLock.lock();
         imgOut->setPixel(x,y,0xFFFFFFFF);
         imgLock.unlock();
      }
    }
  }
}

void ImageProcessor::expandPixelsY(int sx, int ex, int ey, QImage * imgIn, QImage * imgOut)
{
  int sy = 0;
  for(int x = sx;x<ex;x++)
  {
    int endblack = 0;
    for(int y = sy;y<sy+PIXEL_RADIUS && y<ey;y++)
    {
      QColor c(imgIn->pixel(x,y));
      if(c==Qt::black)
      {
        endblack = y+PIXEL_RADIUS;
      }
    }
    for(int y = sy;y<ey;y++)
    {
      int yy = y+PIXEL_RADIUS;
      if(yy<ey)
      {
        QColor c(imgIn->pixel(x,yy));
        if(c==Qt::black)
        {
          endblack = yy+PIXEL_RADIUS;
        }
      }
      if(y<endblack)
      {
         imgLock.lock();
         imgOut->setPixel(x,y,0xFF000000);
          imgLock.unlock();
      }
      else
      {
         imgLock.lock();
         imgOut->setPixel(x,y,0xFFFFFFFF);
         imgLock.unlock();
      }
    }
  }
}


void ImageProcessor::prepareImg(const QImage &image, int sx, int sy, int ex, int ey)
{
  for(int x = sx;x<ex;x++)
  {
    for(int y = sy;y<ey;y++)
    {
      QColor c1(image.pixel(x,y)),c2(oldImage->pixel(x,y));
      //QColor c3(avgImage->pixel(x,y)),c4(avgImage2->pixel(x,y));

      uint gsc1 = (3*c1.red()+3*c1.green()+4*c1.blue())/10;
      uint gsc2 = (3*c2.red()+3*c2.green()+4*c2.blue())/10;
      //uint gsca = firstAvg ? c3.blue() : c4.blue();

      //greyscale difference
      uint g = abs(gsc1 - gsc2);
      //if(avgCmp) g = abs(gsc1 - gsca);

      //count pixels with difference> treshold
      if (g>TRESHOLD) sum++;
      else g = 0;

      imgLock.lock();
      {
        QColor c(g,g,g);
        img.setPixel(x,y,c.rgb());
      }
      imgLock.unlock();



    }
  }
}

ImageProcessor::ImageProcessor(int width, int height): images(0), images2(MAX_FRAMES/2)
{    
  oldImage = new QImage(width,height,QImage::Format_RGB32);  
  expandedImg = new QImage(width,height,QImage::Format_RGB32);
  expandedImgX = new QImage(width,height,QImage::Format_RGB32);
}

QImage ImageProcessor::processImage(const QImage &image)
{
  img = image;

  sum = 0;  
  //prepare
  vector<QFuture<void> > threads;
  int n = QThread::idealThreadCount();
  for(int i=0;i<n;i++)
  {
    threads.push_back(QtConcurrent::run(this,&ImageProcessor::prepareImg, image, (img.width()*i)/n,0,((i+1)*img.width())/n,img.height()));
  }
  for(int i=0;i<n;i++)
  {
    threads[i].waitForFinished();
  }
  threads.clear();
  //expand X
  for(int i=0;i<n;i++)
  {
    threads.push_back(QtConcurrent::run(this,&ImageProcessor::expandPixelsX, (img.height()*i)/n,img.width(),((i+1)*img.height())/n,&img,expandedImgX));
  }
  for(int i=0;i<n;i++)
  {
    threads[i].waitForFinished();
  }
  threads.clear();
  //expand Y
  for(int i=0;i<n;i++)
  {
    threads.push_back(QtConcurrent::run(this,&ImageProcessor::expandPixelsY, (img.width()*i)/n,((i+1)*img.width())/n,img.height(),expandedImgX,expandedImg));
  }
  for(int i=0;i<n;i++)
  {
    threads[i].waitForFinished();
  }

  imgChanged = false;

  //big difference
  if(sum>img.width()*img.height()/RATIO)
  {
    imgChanged = true;    
  }

  delete oldImage;
  oldImage = new QImage(image);

  //return img;
  return *expandedImg;
}

ImageProcessor::~ImageProcessor()
{
  delete oldImage;  
  delete expandedImg;
  delete expandedImgX;
}
