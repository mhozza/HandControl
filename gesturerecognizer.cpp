#include "gesturerecognizer.h"
#include <iostream>
#include <cmath>
#include "gesturemoveleft.h"
#include "gesturemoveright.h"
#include "gesturemoveup.h"
#include "gesturemovedown.h"

int point_subtract(QPoint a, QPoint b)
{
  int x = abs(a.x()-b.x());
  int y = abs(a.y()-b.y());
  return round(sqrt(x*x+y*y));
}

GestureRecognizer::GestureRecognizer()
{
  gestures.clear();
  points.clear();

  addGesture(new GestureMoveLeft());
  addGesture(new GestureMoveRight());
  addGesture(new GestureMoveUp());
  addGesture(new GestureMoveDown());
}

void GestureRecognizer::resetGesture()
{
  points.clear();
}

void GestureRecognizer::addPoint(QPoint p)
{
  cout << points.size() << endl;
  points.push_back(p);
}

Gesture * GestureRecognizer::getGesture()
{
  removeNoise();
  for(vector<Gesture *>::iterator it =  gestures.begin();it !=gestures.end();it++)
  {
    if((*it)->check(points)) return (*it);
  }
  return NULL;
}

void GestureRecognizer::removeNoise()
{  
  if(points.size()<3) return;
  int f = 1, a = 1;
  while(f<points.size() && a<points.size())
  {
    if(point_subtract(points[f-1],points[a])<MAX_POINT_DISTANCE)
    {
      points[f] = points[a];
      f++;
    }
    a++;
  }
  points.resize(f);
}

void GestureRecognizer::addGesture(Gesture * g)
{
  gestures.push_back(g);
}


GestureRecognizer::~GestureRecognizer()
{
  for(typeof(gestures.begin()) it = gestures.begin();it!=gestures.end();it++) delete (*it);
}

