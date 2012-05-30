#include "handrecognizer.h"
#include "imageprocessor.h"

//#define SAVE_HAND

#include <iostream>
#include <fstream>
#include <sstream>
#include <fftw3.h>

using namespace std;

int subtract(QPoint a, QPoint b)
{
  int x = abs(a.x()-b.x());
  int y = abs(a.y()-b.y());
  return round(sqrt(x*x+y*y));
}

HandRecognizer::HandRecognizer()
{
  //index = 1248;
  unsigned sizes[] = {HIDDEN_N, OUT_N};
  //unsigned sizes[] = {HIDDEN_N, HIDDEN_N2, OUT_N};
#ifdef USE_RECURRENT
  net = new DistributedRecurrentNetwork(2,sizes,HIDDEN_N_SIDE, HIDDEN_N_SIDE, N_SIDE, N_SIDE,0);
  net->loadWeights("classifier.r.dat");
#else
  net = new DistributedNeuralNetwork(2,sizes,HIDDEN_N_SIDE, HIDDEN_N_SIDE, N_SIDE, N_SIDE,0);
  net->loadWeights("classifier.dat");
#endif

}

bool HandRecognizer::isSimilarRect(QRect r1, QRect r2)
{
  int min = 9999, max = 0;
  int d = subtract(r1.topLeft(),r2.topLeft());
  min = d<min ? d : min;
  max = d>max ? d : max;
  d = subtract(r1.topRight(),r2.topRight());
  min = d<min ? d : min;
  max = d>max ? d : max;
  d = subtract(r1.bottomLeft(),r2.bottomLeft());
  min = d<min ? d : min;
  max = d>max ? d : max;
  d = subtract(r1.bottomRight(),r2.bottomRight());
  min = d<min ? d : min;
  max = d>max ? d : max;
  if (min<30 && max < 50) return true;
  return false;
}

void HandRecognizer::processRects(queue<pair<QRect,uint> > * q, GrayScaleImage * imgRef, GrayScaleImage * img)
{
  resetHand();  
  while(true)
  {
    rectQueueLock.lock();
    if(q->empty())
    {
        rectQueueLock.unlock();
        continue;
    }

    QRect r = q->front().first;
    uint c = q->front().second;

    if (c == 0)
    {
        rectQueueLock.unlock();
        break;
    }

    q->pop();
    rectQueueLock.unlock();

    //crop and scale image
    GrayScaleImage* imgRefScaled = (GrayScaleImage*)imgRef->copy(r);
    GrayScaleImage* imgScaled = (GrayScaleImage*)img->copy(r);    
    imgScaled->mask(imgRefScaled,true);

    imgScaled->scale(SCALE_SIZE,SCALE_SIZE);

    //fft
    fftw_complex *in = NULL;
    fftw_complex *out = NULL;
    fftw_plan p;
    in = imgScaled->toComplexArray();    
    fftLock.lock();
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_2d(imgScaled->width(), imgScaled->height(), in, out, FFTW_FORWARD ,FFTW_ESTIMATE | FFTW_DESTROY_INPUT);
    fftLock.unlock();
    fftw_execute(p);
    fftLock.lock();
    fftw_destroy_plan(p);
    fftLock.unlock();
    delete in;


    float hand = 0;

    //vygenerovanie vector<float> vstupu pre net
    vector<float> input;
    input.resize(N);
    int i = -1;

    for(unsigned y = 0;y < SCALE_SIZE; y++)
    {
      for(unsigned x = 0;x < SCALE_SIZE; x++)
      {
        i++;
        if(x>=imgRefScaled->width() || y >=imgRefScaled->height()) {
          input[i] = 0;
          continue;
        }
        //input[i] = imgScaled.pixel(x,y);
        input[i] = 1/(1+Utils::cabs(out[x+y*SCALE_SIZE]));       
      }
    }

    //rozpoznanie ruky:    
    hand = net->classify1(input);
#ifdef USE_RECURRENT
    if(hand>HAND_TRESHOLD)
    {
      net->update();
    }
#endif

    if(isSimilarRect(r,handRect)) hand += 0.3;

    if(hand>hand_p)
    {
      hand_p = hand;
      handRect = r;
    }

    fftLock.lock();
    fftw_free(out);
    fftLock.unlock();
    delete imgRefScaled;
    delete imgScaled;    
  }    
}

void HandRecognizer::reset()
{
#ifdef USE_RECURRENT
  net->reset();
#endif
}
