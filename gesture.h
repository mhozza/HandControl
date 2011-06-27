#ifndef GESTURE_H
#define GESTURE_H

#include <QPoint>
#include <QString>

#include <vector>

#include "xfunc.h"

using namespace std;

class Gesture
{  
protected:
  QString txt;
  void sendInput(unsigned long k);
public:
  //setAction();
  Gesture(QString text) : txt(text){}
  QString text();
  virtual bool check(vector<QPoint>) = 0;
  virtual void action() = 0;
};

#endif // GESTURE_H
