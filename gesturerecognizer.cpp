#include "gesturerecognizer.h"
#include <iostream>
#include "gesturemoveleft.h"
#include "gesturemoveright.h"

GestureRecognizer::GestureRecognizer()
{
  gestures.resize(0);
  points.resize(0);

  addGesture(new GestureMoveLeft());
  addGesture(new GestureMoveRight());
}

void GestureRecognizer::resetGesture()
{
  points.resize(0);
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
