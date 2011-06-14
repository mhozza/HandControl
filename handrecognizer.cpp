#include "handrecognizer.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

HandRecognizer::HandRecognizer()
{
  index = 0;
}

void HandRecognizer::processRects(queue<pair<QRect,uint> > * q, QImage * imgRef, QImage * img)
{
  while(true)
  {
    if(q->empty()) continue;
    QRect r = q->front().first;
    uint c = q->front().second;
    if (c == 0) break;
    q->pop();
    //cout << r.left() << " " << r.top() << " " << r.right() << " " << r.bottom() << " " << c << endl;
    stringstream fname;
    fname << "hand_images/hand_" << index << ".pbm";
    index++;

    //crop image
    QImage imgRefScaled = imgRef->copy(r);
    QImage imgScaled = img->copy(r);

    //scale image
    QMatrix m;
    m.scale(SCALE_SIZE/imgRefScaled.width(),SCALE_SIZE/imgRefScaled.height());
    imgRefScaled = imgRefScaled.transformed(m);
    imgScaled = imgScaled.transformed(m);

    //cout << imgScaled.width() << " " << imgScaled.height() << endl;

    ofstream ofs(fname.str().c_str());
    ofs << "P1" << endl;
    ofs << SCALE_SIZE << " " << SCALE_SIZE << endl;
    for(int y = 0;y <= SCALE_SIZE; y++)
    {
      for(int x = 0;x <= SCALE_SIZE; x++)
      {
        if(x>=imgRefScaled.width() || y >=imgRefScaled.height()) {
          cout << 0 << " ";
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
