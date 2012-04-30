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
  index = 4400;
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

void HandRecognizer::processRects(queue<pair<QRect,uint> > * q, GrayScaleImage * imgRef, GrayScaleImage * img, GrayScaleImage * img2, ColorImage * imgcolor)
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
    GrayScaleImage* imgScaled2 = (GrayScaleImage*)img2->copy(r);
    ColorImage* imgColorScaled = (ColorImage*)imgcolor->copy(r);
    imgScaled2->mask(imgRefScaled,true);
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

    float hand = 0;
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

	saveImageBuffer.push_back(imgScaled->saveImageToString());    
    //imgScaled->setImageFromComplexArray(out,SCALE_SIZE,SCALE_SIZE);
    //saveImageBuffer2.push_back(imgScaled->saveImageToString());

    saveImageBuffer3.push_back(imgScaled2->saveImageToString());
    //imgScaled2->setImageFromComplexArray(out2,SCALE_SIZE,SCALE_SIZE);
    //saveImageBuffer4.push_back(imgScaled2->saveImageToString());

    /*
    stringstream ofs;
    stringstream ofs2;    

    for(unsigned y = 0;y < SCALE_SIZE; y++)
    {
      for(unsigned x = 0;x < SCALE_SIZE; x++)
      {        
        if(x>=imgScaled->width() || y >=imgScaled->height())
        {
          ofs << 0 << " ";
          ofs2 << 0 << " ";
          continue;
        }
        ofs << 1/(1+Utils::cabs(out[x+y*SCALE_SIZE])) << " ";
        ofs2 << 1/(1+Utils::cabs(out2[x+y*SCALE_SIZE])) << " ";
      }
      ofs << endl;
      ofs2 << endl;
    }

    saveImageBuffer5.push_back(ofs.str());
    saveImageBuffer6.push_back(ofs2.str());
    */

    if(saveImageBuffer.size()>=SAVEIMAGE_BUFFER_SIZE)
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

    for(unsigned i = 0;i<imageIsHand.size();i++)
    {                
        string fname,fname2,fname3,fname4,fname5,fname6;
        string pathNew = "hand_images/new/";
        string pathOld = "hand_images/old/";

        fname = makeFileName(pathNew,".trn",0,index,'a',imageIsHand[i]);
        fname2 = makeFileName(pathNew,".pbm",0,index,'a',imageIsHand[i]);
        fname3 = makeFileName(pathNew,".trn.pbm",0,index,'a',imageIsHand[i]);
        fname4 = makeFileName(pathOld,".trn",0,index,'a',imageIsHand[i]);
        fname5 = makeFileName(pathOld,".pbm",0,index,'a',imageIsHand[i]);
        fname6 = makeFileName(pathOld,".trn.pbm",0,index,'a',imageIsHand[i]);
        index++;

        ofstream ofs;

        ofs.open(fname2.c_str());
        ofs << saveImageBuffer[i];
        ofs.close();

        /*
        ofs.open(fname3.c_str());
        ofs << saveImageBuffer2[i];
        ofs.close();
        */

        ofs.open(fname5.c_str());
        ofs << saveImageBuffer3[i];
        ofs.close();

        /*
        ofs.open(fname6.c_str());
        ofs << saveImageBuffer4[i];
        ofs.close();

        ofs.open(fname.c_str());
        ofs << saveImageBuffer5[i];
        ofs.close();

        ofs.open(fname4.c_str());
        ofs << saveImageBuffer6[i];
        ofs.close();
        */
    }

    imageIsHand.clear();
    saveImageBuffer.clear();
    //saveImageBuffer2.clear();
    saveImageBuffer3.clear();
    /*saveImageBuffer4.clear();
    saveImageBuffer5.clear();
    saveImageBuffer6.clear();*/

    cout << " [Done]" << endl;
}

unsigned HandRecognizer::getSaveImageBufferSize()
{
    return imageIsHand.size();
}
