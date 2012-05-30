#ifndef HANDRECOGNIZER_H
#define HANDRECOGNIZER_H

#include <queue>
#include <QRect>
#include <QImage>
#include <QMutex>

#include "NeuralNet/neuralnetwork.h"
#include "NeuralNet/distributedrecurrentnetwork.h"
#include "NeuralNet/distributedneuralnetwork.h"

#include "grayscaleimage.h"
#include "colorimage.h"

using namespace std;
using namespace NeuralNET;

#define SCALE_SIZE 128
#define N SCALE_SIZE*SCALE_SIZE
#define N_SIDE SCALE_SIZE
#define OUT_N 1
#define HIDDEN_N_SIDE 4
#define HIDDEN_N HIDDEN_N_SIDE*HIDDEN_N_SIDE*11
#define HIDDEN_N2 7
#define HAND_TRESHOLD 0.85
#define SAVEIMAGE_BUFFER_SIZE 300
#define USE_RECURRENT

class HandRecognizer
{
#ifdef USE_RECURRENT
  RecurrentNetwork * net;
#else
  NeuralNetwork * net;
#endif
  int index;  
  float hand_p;
  QRect handRect;  
  inline void resetHand(){hand_p = 0;}
  bool isSimilarRect(QRect r1, QRect r2);  
  vector<string> saveImageBuffer;
  QMutex fftLock;
public:
  QMutex rectQueueLock;
  HandRecognizer();
  void processRects(queue<pair<QRect,uint> > * q, GrayScaleImage * img_ref, GrayScaleImage * img);
  inline bool isHand(){return hand_p>HAND_TRESHOLD;}
  inline QRect getHandRect(){return handRect;}
  inline float getHandP(){return hand_p;}
  void reset();
};

#endif // HANDRECOGNIZER_H
