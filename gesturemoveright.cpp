#include "gesturemoveright.h"

#include <iostream>

void GestureMoveRight::action()
{
  sendInput(0x0020);//space
  cout << "move right" << endl;
}

bool GestureMoveRight::check(vector<QPoint> points)
{
  //TODO: rozumnejsie rozpoznavanie
  if(points.size()<5) return false;
  if(points.front().x()-points.back().x()<60) return false;
  int maxy = points.front().y(), miny = points.front().y();
  for(vector<QPoint>::iterator it = points.begin();it != points.end();it++)
  {
    maxy = it->y() > maxy ? it->y() : maxy;
    miny = it->y() < miny ? it->y() : miny;
  }
  if(maxy-miny>=60) return false;
  return true;
}
