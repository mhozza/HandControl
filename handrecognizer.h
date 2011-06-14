#ifndef HANDRECOGNIZER_H
#define HANDRECOGNIZER_H

#include <queue>
#include <QRect>
#include <QImage>

using namespace std;

#define SCALE_SIZE 80.0

class HandRecognizer
{
  int index;
public:
    HandRecognizer();
    void processRects(queue<pair<QRect,uint> > * q, QImage * img_ref, QImage * img);
};

#endif // HANDRECOGNIZER_H
