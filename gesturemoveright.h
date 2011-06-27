#ifndef GESTUREMOVERIGHT_H
#define GESTUREMOVERIGHT_H

#include "gesture.h"

class GestureMoveRight : public Gesture
{  
public:  
  GestureMoveRight(): Gesture("Move Right"){}
  bool check(vector<QPoint> points);
  void action();
};

#endif // GESTUREMOVERIGHT_H
