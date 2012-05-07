/*
 *    HandControl - Hand gesture recognition
 *    Copyright (C) 2011  Michal Hozza (mhozza@gmail.com)
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTILS_H
#define UTILS_H

#include <QImage>
#include <QColor>
#include <sstream>
#include <fstream>
#include <fftw3.h>
#include <cmath>
#include <vector>
#include "utils.h"
#include <queue>
#include <iostream>

using namespace std;

class Utils
{
  Utils(){}
public:
  static void saveImage(QImage img, int index);
  static double cabs(fftw_complex v);
};

struct IndexInfo
{
    uint seqIndex, frameIndex;
    char partIndex;
    IndexInfo(uint seqIndex,uint frameIndex, char partIndex):seqIndex(seqIndex),
        frameIndex(frameIndex), partIndex(partIndex)
    {
        //if(partIndex!=0) cerr << seqIndex << "_" << frameIndex << partIndex << endl;
    }
};

typedef queue<pair<pair<QRect,uint>, IndexInfo> > RectQueue;

/*
template <class T> class Matrix
{
  Matrix();
public:
  static vector<vector<T> > createIdentity(int w, int h);
  //static vector<vector<T> > vector2matrix(vector<T> v);
  static vector<vector<T> > transpone(vector<vector<T> >);

};

template <class T> vector<T> operator+(vector<T> a, vector<T> b);
template <class T> vector<vector<T> > operator+(vector<vector<T> > a, vector<vector<T> > b);

template <class T> T operator*(vector<T> a, vector<T> b); //!!skalarny sucin
template <class T> vector<vector<T> > operator+(vector<vector<T> > a, vector<vector<T> > b); //!!maticove nasobenie
*/

#endif // UTILS_H
