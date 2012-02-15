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
  index = 1248;
  unsigned sizes[] = {HIDDEN_N, HIDDEN_N2, OUT_N};
  net = new DistributedNeuralNetwork(3,sizes,HIDDEN_N_SIDE, HIDDEN_N_SIDE, N_SIDE, N_SIDE,0);
  //net = new NeuralNetwork(2,sizes,N,0);
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
    imgScaled.mask(imgRefScaled,true);
    //Utils::saveImage(imgScaled,index);
    //imgRefScaled.scale(SCALE_SIZE,SCALE_SIZE);
    imgScaled.scale(SCALE_SIZE,SCALE_SIZE);

    //fft
//    double *in = NULL;
    fftw_complex *in = NULL;
    fftw_complex *out = NULL;
    fftw_plan p;
    in = imgScaled.toComplexArray();
    //in = imgScaled.toDoubleArray();
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_2d(imgScaled.width(), imgScaled.height(), in, out, FFTW_FORWARD ,FFTW_ESTIMATE | FFTW_DESTROY_INPUT);
    //p = fftw_plan_dft_r2c_2d(imgScaled.width(), imgScaled.height(), in, out, FFTW_ESTIMATE | FFTW_DESTROY_INPUT);
    fftw_execute(p); // repeat as needed
    fftw_destroy_plan(p);    
    //fftw_free(in);

    //vygenerovanie vector<float> vstupu pre net
    vector<float> input;
    input.resize(N);
    int i = -1;

    for(unsigned y = 0;y < SCALE_SIZE; y++)
    {
      for(unsigned x = 0;x < SCALE_SIZE; x++)
      {
        i++;
        if(x>=imgRefScaled.width() || y >=imgRefScaled.height()) {
          input[i] = 0;
          continue;
        }
        //input[i] = imgScaled.pixel(x,y);
        input[i] = 1/(1+Utils::cabs(out[x+y*SCALE_SIZE]));

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
    stringstream fname,fname2,fname3;
    //index = 0;
    fname << "hand_images/"<< ((hand>0.5) ? "hand" : "other") << "_" << index << ".trn";
    fname2 << "hand_images/"<< ((hand>0.5) ? "hand" : "other") << "_" << index << ".pbm";
    fname3 << "hand_images/"<< ((hand>0.5) ? "hand" : "other") << "_" << index << ".trn.pbm";
    index++;

    ofstream ofs(fname.str().c_str());
    for(unsigned y = 0;y < SCALE_SIZE; y++)
    {
      for(unsigned x = 0;x < SCALE_SIZE; x++)
      {
        i++;
        if(x>=imgRefScaled.width() || y >=imgRefScaled.height()) {
            ofs << 0 << " ";
          continue;
        }
        //input[i] = imgScaled.pixel(x,y);
        ofs << 1/(1+Utils::cabs(out[x+y*SCALE_SIZE])) << " ";
      }
      ofs << endl;
    }
    Utils::saveImage(imgScaled,index,fname2.str());
    imgScaled.setImageFromComplexArray(out,SCALE_SIZE,SCALE_SIZE);
    Utils::saveImage(imgScaled,index,fname3.str());

#endif    
    fftw_free(out);
  }    
}
