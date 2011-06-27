#include "gesturemovedown.h"

#include <iostream>

void GestureMoveDown::action()
{
  sendInput(0xff0d);//Enter
  cout << "move down" << endl;
}

bool GestureMoveDown::check(vector<QPoint> points)
{
  //TODO: rozumnejsie rozpoznavanie
  if(points.size()<5) return false;
  if(points.back().y()-points.front().y()<60) return false;
  int maxy = points.front().x(), miny = points.front().x();
  for(vector<QPoint>::iterator it = points.begin();it != points.end();it++)
  {
    maxy = it->x() > maxy ? it->x() : maxy;
    miny = it->x() < miny ? it->x() : miny;
  }
  if(maxy-miny>=60) return false;
  return true;
}
