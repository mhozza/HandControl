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
#include <sstream>

#define WHITE 0xFF
#define BLACK 0

using namespace std;

void ImageProcessor::medianFilterX(int sy, int ex, int ey, HCImage * imgIn, HCImage * imgOut)
{
  int sx = 0;
  for(int y = sy;y<ey;y++)
  {
    for(int x = sx+1;x<ex-1;x++)
    {
      uchar c1 = imgIn->pixel(x-1,y);
      uchar c2 = imgIn->pixel(x,y);
      uchar c3 = imgIn->pixel(x+1,y);

      uchar c = median3(c1,c2,c3);

      imgLock.lock();
      imgOut->setPixel(x,y,c);
      imgLock.unlock();
    }
  }
}

void ImageProcessor::medianFilterY(int sx, int ex, int ey, HCImage * imgIn, HCImage * imgOut)
{
  int sy = 0;
  for(int x = sx;x<ex;x++)
  {
    for(int y = sy+1;y<ey-1;y++)
    {
        uchar c1 = imgIn->pixel(x,y-1);
        uchar c2 = imgIn->pixel(x,y);
        uchar c3 = imgIn->pixel(x,y+1);

        uchar c = median3(c1,c2,c3);


      imgLock.lock();
      imgOut->setPixel(x,y,c);
      imgLock.unlock();
    }
  }
}


void ImageProcessor::expandPixelsX(int sy, int ex, int ey, HCImage * imgIn, HCImage * imgOut)
{
  int sx = 0;
  for(int y = sy;y<ey;y++)
  {
    int endblack = 0;
    for(int x = sx;x<sx+PIXEL_RADIUS && x<ex;x++)
    {
      uchar c = imgIn->pixel(x,y);
      if(c!=WHITE)
      {
        endblack = x+PIXEL_RADIUS;
      }
    }
    for(int x = sx;x<ex;x++)
    {
      int xx = x+PIXEL_RADIUS;
      if(xx<ex)
      {
        uchar c = imgIn->pixel(xx,y);
        if(c!=WHITE)
        {
          endblack = xx+PIXEL_RADIUS;
        }
      }
      if(x<endblack)
      {
         imgLock.lock();
         imgOut->setPixel(x,y,0);
         imgLock.unlock();
      }
      else
      {
         imgLock.lock();
         imgOut->setPixel(x,y,0xFF);
         imgLock.unlock();
      }
    }
  }
}

void ImageProcessor::expandPixelsY(int sx, int ex, int ey, HCImage * imgIn, HCImage * imgOut)
{
  int sy = 0;
  for(int x = sx;x<ex;x++)
  {
    int endblack = 0;
    for(int y = sy;y<sy+PIXEL_RADIUS && y<ey;y++)
    {
      uchar c = imgIn->pixel(x,y);
      if(c!=WHITE)
      {
        endblack = y+PIXEL_RADIUS;
      }
    }
    for(int y = sy;y<ey;y++)
    {
      int yy = y+PIXEL_RADIUS;
      if(yy<ey)
      {
        uchar c = imgIn->pixel(x,yy);
        if(c!=WHITE)
        {
          endblack = yy+PIXEL_RADIUS;
        }
      }
      if(y<endblack)
      {
         imgLock.lock();
         imgOut->setPixel(x,y,0);
         imgLock.unlock();
      }
      else
      {
         imgLock.lock();
         imgOut->setPixel(x,y,0xFF);
         imgLock.unlock();
      }
    }
  }
}



QRect ImageProcessor::segment(int sx, int sy, uchar color, HCImage * image, QRect rect)
{
  queue<pair<int,int> > f;
  f.push(make_pair(sx,sy));
  while(!f.empty())
  {
      int x = f.front().first;
      int y = f.front().second;
      f.pop();
      if(x<0 || x>=image->width()) continue;
      if(y<0 || y>=image->height()) continue;
      if(image->pixel(x,y)!=0) continue;
      image->setPixel(x,y,color);
      if(x<rect.left()) rect.setLeft(x);
      if(x>rect.right()) rect.setRight(x);
      if(y<rect.top()) rect.setTop(y);
      if(y>rect.bottom()) rect.setBottom(y);
      f.push(make_pair(x+1,y));
      f.push(make_pair(x-1,y));
      f.push(make_pair(x,y+1));
      f.push(make_pair(x,y-1));
  }
  if(rect.height()>(3*rect.width())/2)
    rect.setHeight(rect.width());
  return rect;
}

void ImageProcessor::prepareImg(HCImage &image, int sx, int sy, int ex, int ey)
{
  for(int x = sx;x<ex;x++)
  {
    for(int y = sy;y<ey;y++)
    {
      int gsc1 = image.pixel(x,y);
      int gsc2 = oldImage->pixel(x,y);

      //greyscale difference
      uint g = abs(gsc1 - gsc2);

      //count pixels with difference> treshold
      if (g>TRESHOLD) sum++;
      else g = 0;

      g=0xFF - g;      

      imgLock.lock();
      img.setPixel(x,y,g);
      imgLock.unlock();
    }
  }
}

ImageProcessor::ImageProcessor(int width, int height, HandRecognizer*  handRecognizer): images(0), images2(MAX_FRAMES/2), index(0)
{
  oldImage = new HCImage(width,height);
  expandedImg = new HCImage(width,height);
  expandedImgX = new HCImage(width,height);
  this->handRecognizer = handRecognizer;
}

HCImage ImageProcessor::processImage(const HCImage &image)
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

  //Segment and recognize
  uint color = 1;

  while(!rectQueue.empty()) rectQueue.pop(); //empty the queue

  for(int i=0;i<n-1 || i<1;i++)
  {
    threads.push_back(QtConcurrent::run(handRecognizer,&HandRecognizer::processRects, &rectQueue, expandedImg, (HCImage*) &img,&imgLock));
  }

  for(int y = 0;y<expandedImg->height();y++)
  {
    for(int x = 0;x<expandedImg->width();x++)
    {
      if(expandedImg->pixel(x,y)==0)
      {        
        color = 1+rand()%254;
        QRect r(x,y,0,0);        
        r = segment(x,y,color,expandedImg,r);
        //if(r.width()!=0) color++;
        if(r.width()>=MIN_RECT_SIZE && r.height()>=MIN_RECT_SIZE && r.width()<=MAX_RECT_SIZE && r.height() <= MAX_RECT_SIZE)
        {
          handRecognizer->rectQueueLock.lock();
          rectQueue.push(make_pair(r,color));
          handRecognizer->rectQueueLock.unlock();
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
  oldImage = new HCImage(image);

  //return image;
  //Utils::saveImage(img,index++);
  //return img;
  return *expandedImg;
}

ImageProcessor::~ImageProcessor()
{
  delete oldImage;  
  delete expandedImg;
  delete expandedImgX;
}
