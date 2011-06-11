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

#include "handdetector.h"

HandDetector::HandDetector()
{
}

QPoint HandDetector::getHand(const QImage *img)
{
  unsigned sum = 0;
  float avgx, avgy;
  for(int y = 0;y<img->height();y++)
  {
    for(int x = 0;x<img->width();x++)
    {
      if(img->pixel(x,y)==0xFF000000)
      {
        sum++;
        avgx = ((sum-1)*avgx+x)/sum;
        avgy = ((sum-1)*avgy+y)/sum;
      }
    }
  }
  QPoint p(round(avgx),round(avgy));
  return p;
}
