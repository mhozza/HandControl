#ifndef GESTUREMOVELEFT_H
#define GESTUREMOVELEFT_H

#include "gesture.h"

class GestureMoveLeft : public Gesture
{
public:
  //GestureMoveLeft();
  bool check(vector<QPoint> points);
  void action();
};

#endif // GESTUREMOVELEFT_H
