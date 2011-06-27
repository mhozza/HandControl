#ifndef GESTUREMOVEDOWN_H
#define GESTUREMOVEDOWN_H

#include "gesture.h"

class GestureMoveDown : public Gesture
{  
public:
  GestureMoveDown(): Gesture("Move Down"){}
  bool check(vector<QPoint> points);
  void action();
};

#endif // GESTUREMOVEDOWN_H
