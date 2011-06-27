#include "gesturemoveup.h"

#include <iostream>

void GestureMoveUp::action()
{
  sendInput(0xff1b);//Escape
  cout << "move up" << endl;
}

bool GestureMoveUp::check(vector<QPoint> points)
{
  //TODO: rozumnejsie rozpoznavanie
  if(points.size()<MIN_POINT_COUNT) return false;
  if(points.front().y()-points.back().y()<(MIN_LENGTH*1.5)) return false;
  int maxy = points.front().x(), miny = points.front().x();
  for(vector<QPoint>::iterator it = points.begin();it != points.end();it++)
  {
    maxy = it->x() > maxy ? it->x() : maxy;
    miny = it->x() < miny ? it->x() : miny;
  }
  if(maxy-miny>=MAX_VARIANCE) return false;
  return true;
}
