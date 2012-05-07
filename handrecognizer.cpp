#include "handrecognizer.h"

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

//void HandRecognizer::processRects(queue<pair<QRect,uint> > * q, GrayScaleImage * imgRef, GrayScaleImage * img, GrayScaleImage * img2, ColorImage * imgcolor)
void HandRecognizer::processRects(RectQueue *q, GrayScaleImage *imgRef, GrayScaleImage *img, GrayScaleImage *img2, ColorImage *imgcolor)
{
  //TODO: waiting namiest zaneprazdneneho cakania
  resetHand();
  while(true)
  {
    rectQueueLock.lock();
    if(q->empty())
    {
        rectQueueLock.unlock();
        continue;
    }

    QRect r = q->front().first.first;
    uint c = q->front().first.second;
    IndexInfo index = q->front().second;

    if (c == 0)
    {
        rectQueueLock.unlock();
        break;
    }
    //cerr << r.x() << "," << r.y() << ":" << index.frameIndex << index.partIndex << endl;

    q->pop();
    rectQueueLock.unlock();

    //crop and scale image
    GrayScaleImage* imgRefScaled = (GrayScaleImage*)imgRef->copy(r);
    GrayScaleImage* imgScaled = (GrayScaleImage*)img->copy(r);
    GrayScaleImage* imgScaled2 = (GrayScaleImage*)img2->copy(r);
    ColorImage* imgColorScaled = (ColorImage*)imgcolor->copy(r);
    //imgScaled2->mask(imgRefScaled,true);
    ColorImage * handMask = (ColorImage*)imgColorScaled->getAdaptiveFloodFillSelectionMask(0.5*r.width(),0.55*r.height(),23,0.25,0.55);
    imgScaled->mask(handMask->toGrayScale());
    //GrayScaleImage* imgScaled = handMask->toGrayScale();
    delete handMask;    

    imgScaled->scale(SCALE_SIZE,SCALE_SIZE);
    imgScaled2->scale(SCALE_SIZE,SCALE_SIZE);

    //fft
    fftw_complex *in = NULL;
    fftw_complex *out = NULL;
    fftw_plan p;
    in = imgScaled->toComplexArray();
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p = fftw_plan_dft_2d(imgScaled->width(), imgScaled->height(), in, out, FFTW_FORWARD ,FFTW_ESTIMATE | FFTW_DESTROY_INPUT);
    fftw_execute(p); // repeat as needed
    fftw_destroy_plan(p);
    delete in;

    fftw_complex *in2 = NULL;
    fftw_complex *out2 = NULL;
    fftw_plan p2;
    in2 = imgScaled2->toComplexArray();
    out2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    p2 = fftw_plan_dft_2d(imgScaled2->width(), imgScaled2->height(), in2, out2, FFTW_FORWARD ,FFTW_ESTIMATE | FFTW_DESTROY_INPUT);
    fftw_execute(p2); // repeat as needed
    fftw_destroy_plan(p2);
    delete in2;

    float hand = 1;
    /*
    //vygenerovanie vector<float> vstupu pre net
    vector<float> input;
    input.resize(N);
    int i = -1;

    for(unsigned y = 0;y < SCALE_SIZE; y++)
    {
      for(unsigned x = 0;x < SCALE_SIZE; x++)
      {
        i++;
        if(x>=imgScaled->width() || y >=imgScaled->height()) {
          input[i] = 0;
          continue;
        }
        input[i] = 1/(1+Utils::cabs(out2[x+y*SCALE_SIZE]));
      }
    }

    //rozpoznanie ruky:
    hand = net->classify1(input);

    if(isSimilarRect(r,handRect)) hand += 0.3;

    if(hand>hand_p)
    {
      hand_p = hand;
      handRect = r;
    }
    */

#ifdef SAVE_HAND
    saveLock.lock();
    imageIsHand.push_back(hand>0.5);
    indexInfoBuffer.push_back(index);
    saveImageBuffer1.push_back(imgScaled->saveImageToString());
    saveImageBuffer2.push_back(imgScaled2->saveImageToString());


    if(saveImageBuffer1.size()>=SAVEIMAGE_BUFFER_SIZE)
    {
        saveAllImages();
    }
    saveLock.unlock();
#endif    
    fftw_free(out);
    fftw_free(out2);

    delete imgRefScaled;
    delete imgScaled;
    delete imgScaled2;
    delete imgColorScaled;
  }    
}

string HandRecognizer::makeFileName(string path, string suffix, unsigned seqindex, unsigned frameindex, char partindex, bool hand)
{
    stringstream fname;
    //fname << path << (hand ? "hand" : "other") << "_" << index << suffix;
    fname << path << "img_" << seqindex << "_" << frameindex << partindex << suffix;
    return fname.str();
}

void HandRecognizer::saveAllImages()
{
    cout << "Saving...";

    for(unsigned i = 0;i<saveImageBuffer1.size();i++)
    {                
        string fname1,fname2;
        string pathNew = "hand_images/new/";
        string pathOld = "hand_images/old/";

        fname1 = makeFileName(pathNew,".pbm", indexInfoBuffer[i].seqIndex, indexInfoBuffer[i].frameIndex, indexInfoBuffer[i].partIndex,imageIsHand[i]);
        fname2 = makeFileName(pathOld,".pbm", indexInfoBuffer[i].seqIndex, indexInfoBuffer[i].frameIndex, indexInfoBuffer[i].partIndex,imageIsHand[i]);

        ofstream ofs;

        ofs.open(fname1.c_str());
        ofs << saveImageBuffer1[i];
        ofs.close();

        ofs.open(fname2.c_str());
        ofs << saveImageBuffer2[i];
        ofs.close();
    }

    imageIsHand.clear();
    saveImageBuffer1.clear();
    saveImageBuffer2.clear();

    cout << " [Done]" << endl;
}

unsigned HandRecognizer::getSaveImageBufferSize()
{
    return imageIsHand.size();
}
