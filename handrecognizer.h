#ifndef HANDRECOGNIZER_H
#define HANDRECOGNIZER_H

#include <queue>
#include <QRect>
#include <QImage>
#include <QMutex>

#include "NeuralNet/neuralnetwork.h"

using namespace std;
using namespace NeuralNET;

#define SCALE_SIZE 80.0
#define N SCALE_SIZE*SCALE_SIZE
#define N_SIDE SCALE_SIZE
#define OUT_N 1
#define HIDDEN_N 17
#define TRESHOLD 0.95

class HandRecognizer
{
  NeuralNetwork * net;  
  int index;  
  float hand_p;
  QRect handRect;  
  inline void resetHand(){hand_p = 0;}
  bool isSimilarRect(QRect r1, QRect r2);
public:
  HandRecognizer();
  void processRects(queue<pair<QRect,uint> > * q, QImage * img_ref, QImage * img, QMutex *imglock);
  inline bool isHand(){return hand_p>TRESHOLD;}
  inline QRect getHandRect(){return handRect;}
  inline float getHandP(){return hand_p;}
};

#endif // HANDRECOGNIZER_H
