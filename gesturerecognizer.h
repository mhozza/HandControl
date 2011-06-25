#ifndef GESTURERECOGNIZER_H
#define GESTURERECOGNIZER_H

#include <QPoint>
#include <QPainter>

#include <vector>
#include <algorithm>
#include <iostream>

#include "gesture.h"

using namespace std;

class GestureRecognizer
{
    vector<QPoint> points;
    vector<Gesture *> gestures;
    void removeNoise();
public:    
    GestureRecognizer();
    ~GestureRecognizer();
    //GestureRecognizer(vector<Gesture> g);
    void addGesture(Gesture * g);
    void resetGesture();
    void addPoint(QPoint p);
    Gesture * getGesture();
    //debug
    inline void drawPoints(QPainter * p)
    {
      //cout << points.size() << endl;
      for(int i = 1; i<points.size();i++)
      {
        //p->drawPoint(points[i]);
        p->drawLine(points[i-1],points[i]);
      }
    }
};

#endif // GESTURERECOGNIZER_H
