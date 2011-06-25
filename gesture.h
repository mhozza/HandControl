#ifndef GESTURE_H
#define GESTURE_H

#include <QPoint>

#include <vector>

#include "xfunc.h"

using namespace std;

class Gesture
{
protected:
  void sendInput(int k);
public:
  //setAction();
  //Gesture(action);
  virtual bool check(vector<QPoint>) = 0;
  virtual void action() = 0;
};

#endif // GESTURE_H
