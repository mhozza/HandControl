#ifndef GESTUREMOVEUP_H
#define GESTUREMOVEUP_H

#include "gesture.h"

class GestureMoveUp : public Gesture
{  
public:
  GestureMoveUp(): Gesture("Move Up"){}
  bool check(vector<QPoint> points);
  void action();
};

#endif // GESTUREMOVEUP_H
