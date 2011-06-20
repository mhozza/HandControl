#ifndef HANDRECOGNIZER_H
#define HANDRECOGNIZER_H

#include <queue>
#include <QRect>
#include <QImage>

#include "NeuralNet/neuralnetwork.h"

using namespace std;
using namespace NeuralNET;

#define SCALE_SIZE 80.0
#define N SCALE_SIZE*SCALE_SIZE
#define N_SIDE SCALE_SIZE
#define OUT_N 1
#define ALPHA 0.25
#define TRESHOLD 0.85


class HandRecognizer
{
  NeuralNetwork * net;
  //QMutex lock;
  int index;  
  float hand_p;
  QRect handRect;
  template<class T> inline T first(vector<T> v)
  {
    return v[0];
  }
  inline void resetHand(){hand_p = 0;}

public:
    HandRecognizer();    
    void processRects(queue<pair<QRect,uint> > * q, QImage * img_ref, QImage * img);
    inline bool isHand(){return hand_p>TRESHOLD;}
    inline QRect getHandRect(){return handRect;}
    inline float getHandP(){return hand_p;}
};

#endif // HANDRECOGNIZER_H
