#include "gesturerecognizer.h"
#include <iostream>
#include <cmath>
#include "gesturemoveleft.h"
#include "gesturemoveright.h"

/*int subtract(QPoint a, QPoint b)
{
  int x = abs(a.x()-b.x());
  int y = abs(a.y()-b.y());
  return round(sqrt(x*x+y*y));
}*/

GestureRecognizer::GestureRecognizer()
{
  gestures.clear();
  points.clear();

  addGesture(new GestureMoveLeft());
  addGesture(new GestureMoveRight());
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
  //if(points.size()<3) return;
  //for()
}

void GestureRecognizer::addGesture(Gesture * g)
{
  gestures.push_back(g);
}


GestureRecognizer::~GestureRecognizer()
{
  for(typeof(gestures.begin()) it = gestures.begin();it!=gestures.end();it++) delete (*it);
}
