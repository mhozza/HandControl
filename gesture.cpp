#include "gesture.h"

void Gesture::sendInput(unsigned long k)
{
  sendKey(k);
}

QString Gesture::text()
{
  return txt;
}
