#include "gesturemoveleft.h"

#include <iostream>

void GestureMoveLeft::action()
{  
  sendInput(0xff08);//backspace
  cout << "move left" << endl;
}

bool GestureMoveLeft::check(vector<QPoint> points)
{
  //TODO: rozumnejsie rozpoznavanie
  if(points.size()<MIN_POINT_COUNT) return false;
  if(points.back().x()-points.front().x()<MIN_LENGTH) return false;
  int maxy = points.front().y(), miny = points.front().y();
  for(vector<QPoint>::iterator it = points.begin();it != points.end();it++)
  {
    maxy = it->y() > maxy ? it->y() : maxy;
    miny = it->y() < miny ? it->y() : miny;
  }
  if(maxy-miny>=MAX_VARIANCE) return false;
  return true;
}
