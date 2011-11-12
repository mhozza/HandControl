#include "handrecognizer.h"
#include "imageprocessor.h"

#define SAVE_HAND

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
  index = 860;
  unsigned sizes[] = {HIDDEN_N,OUT_N};
  net = new NeuralNetwork(2,sizes,N,0);
  net->loadWeights("classifier.dat");
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

void HandRecognizer::processRects(queue<pair<QRect,uint> > * q, HCImage * imgRef, HCImage * img, QMutex *imglock)
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
    HCImage imgRefScaled = imgRef->copy(r);
    HCImage imgScaled = img->copy(r);
    Utils::saveImage(imgScaled,index);
    imgRefScaled.scale(SCALE_SIZE,SCALE_SIZE);
    imgScaled.scale(SCALE_SIZE,SCALE_SIZE);

    //fft
    fftw_complex *in = NULL, *out = NULL;
    fftw_plan p;
    in = imgScaled.toComplexArray();
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_2d(imgScaled.width(),imgScaled.height(), in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p); // repeat as needed
    fftw_destroy_plan(p);
    fftw_free(in);
    fftw_free(out);

    //vygenerovanie vector<float> vstupu pre net
    vector<float> input;
    input.resize(N);
    int i = -1;

    for(int y = 0;y < SCALE_SIZE; y++)
    {
      for(int x = 0;x < SCALE_SIZE; x++)
      {
        i++;
        if(x>=imgRefScaled.width() || y >=imgRefScaled.height()) {
          input[i] = 0;
          continue;
        }
        input[i] = imgScaled.pixel(x,y);
        /*if(imgRefScaled.pixel(x,y)==c)
        {
          if(imgScaled.pixel(x,y)!=Qt::white)
            input[i] = 1;
          else
            input[i] = 0;
        }
        else
        {
          input[i] = 0;
        }*/
      }
    }

    //rozpoznanie ruky:    
    float hand = net->classify1(input);

    if(isSimilarRect(r,handRect)) hand += 0.3;

    if(hand>hand_p)
    {
      hand_p = hand;
      handRect = r;
    }
#ifdef SAVE_HAND
    //zapis do suboru
    stringstream fname;
    //index = 0;
    fname << "hand_images/"<< ((hand>0.5) ? "hand" : "other") << "_" << index << ".pbm";
    index++;

    ofstream ofs(fname.str().c_str());
    ofs << "P2" << endl;
    ofs << SCALE_SIZE << " " << SCALE_SIZE << endl;
    ofs << 256 << endl;
    for(int y = 0;y < SCALE_SIZE; y++)
    {
      for(int x = 0;x < SCALE_SIZE; x++)
      {
        if(x>=imgRefScaled.width() || y >=imgRefScaled.height()) {
          ofs << 255 << " ";
          continue;
        }
        //if(imgRefScaled.pixel(x,y)==c)
        {
          //ofs << (int)imgScaled.pixel(x,y) << " ";
            ofs << (int) << " ";
        }
        /*else
        {
          ofs << 255 << " ";
        }*/
      }
      ofs << endl;
    }
#endif
  }    
}
