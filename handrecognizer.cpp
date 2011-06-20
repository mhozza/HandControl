#include "handrecognizer.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


HandRecognizer::HandRecognizer()
{
  index = 860;
  unsigned sizes[] = {12,OUT_N};
  net = new NeuralNetwork(2,sizes,N,ALPHA);
  net->loadWeights("classifier.dat");
}

void HandRecognizer::processRects(queue<pair<QRect,uint> > * q, QImage * imgRef, QImage * img)
{
  resetHand();
  while(true)
  {
    if(q->empty()) continue;
    QRect r = q->front().first;
    uint c = q->front().second;    
    if (c == 0) break;
    q->pop();
    //cout << r.left() << " " << r.top() << " " << r.right() << " " << r.bottom() << " " << c << endl;

    //crop image
    QImage imgRefScaled = imgRef->copy(r);
    QImage imgScaled = img->copy(r);

    //scale image
    QMatrix m;
    m.scale(SCALE_SIZE/imgRefScaled.width(),SCALE_SIZE/imgRefScaled.height());
    imgRefScaled = imgRefScaled.transformed(m);
    imgScaled = imgScaled.transformed(m);


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
        if(imgRefScaled.pixel(x,y)==c)
        {
          if(imgScaled.pixel(x,y)!=Qt::white)
            input[i] = 1;
          else
            input[i] = 0;
        }
        else
        {
          input[i] = 0;
        }
      }
    }

    //rozpoznanie ruky:
    float hand = net->classify1(input);

    if(hand>hand_p)
    {
      hand_p = hand;
      handRect = r;
    }

    //zapis do suboru
    stringstream fname;
    //index = 0;
    fname << "hand_images/"<< ((hand>0.5) ? "hand" : "other") << "_" << index << ".pbm";
    index++;

    ofstream ofs(fname.str().c_str());
    ofs << "P1" << endl;
    ofs << SCALE_SIZE << " " << SCALE_SIZE << endl;
    for(int y = 0;y < SCALE_SIZE; y++)
    {
      for(int x = 0;x < SCALE_SIZE; x++)
      {
        if(x>=imgRefScaled.width() || y >=imgRefScaled.height()) {
          ofs << 0 << " ";
          continue;
        }
        if(imgRefScaled.pixel(x,y)==c)
        {
          if(imgScaled.pixel(x,y)!=Qt::white)
            ofs << 1 << " ";
          else
            ofs << 0 << " ";
        }
        else
        {
          ofs << 0 << " ";
        }
      }
      ofs << endl;
    }
  }
}
