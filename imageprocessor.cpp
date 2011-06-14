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
      if(c!=Qt::white)
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
        if(c!=Qt::white)
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
      if(c!=Qt::white)
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
        if(c!=Qt::white)
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

QRect ImageProcessor::segment(int x, int y, uint color, QImage * image, QRect rect)
{
  if(x<0 ||  x>=image->width()) return rect;
  if(y<0 ||  y>=image->height()) return rect;
  if(image->pixel(x,y)!=0xff000000) return rect;
  image->setPixel(x,y,color);
  if(x<rect.left()) rect.setLeft(x);
  if(x>rect.right()) rect.setRight(x);
  if(y<rect.top()) rect.setTop(y);
  if(y>rect.bottom()) rect.setBottom(y);
  rect = segment(x+1,y,color,image,rect);
  rect = segment(x-1,y,color,image,rect);
  rect = segment(x,y+1,color,image,rect);
  rect = segment(x,y-1,color,image,rect);
  return rect;
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

      g=0xFF - g;

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
  threads.clear();
  uint color = 0xff000000;

  while(!rectQueue.empty()) rectQueue.pop(); //empty the queue

  for(int i=0;i<n-1 || i<1;i++)
  {
    threads.push_back(QtConcurrent::run(&handRecognizer,&HandRecognizer::processRects, &rectQueue, expandedImg, &img));
  }

  for(int y = 0;y<expandedImg->height();y++)
  {
    for(int x = 0;x<expandedImg->width();x++)
    {
      if(expandedImg->pixel(x,y)==0xff000000)
      {
        //color++;
        color = 0xFF000001+rand()%0xFFFFFF;
        QRect r(x,y,0,0);
        r = segment(x,y,color,expandedImg,r);
        cout << "Q:" << rectQueue.size() << endl;
        if(r.width()>=MIN_RECT_SIZE && r.height()>=MIN_RECT_SIZE && r.width()<=MAX_RECT_SIZE && r.height() <= MAX_RECT_SIZE)
        {
          rectQueue.push(make_pair(r,color));
        }
      }
    }
  }

  QRect tmprect;
  rectQueue.push(make_pair(tmprect,0));

  for(int i=0;i<n-1 || i<1;i++)
  {
    threads[i].waitForFinished();
  }
  threads.clear();

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
