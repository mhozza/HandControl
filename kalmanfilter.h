/*
 *    Copyright (C) 2012  Michal Hozza (mhozza@gmail.com)
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

#ifndef KALMANNFILTER_H
#define KALMANNFILTER_H

#include <vector>
#include <kalman/kfilter.hpp>
#include "hcimage.h"

using namespace std;
using namespace Kalman;

class KalmanFilter : private KFilter<double,0,false,false,true>
{
  double percentvar;
  double gain;
  int width, height, dimension;
  //vector<double> stackslice, filteredslice, noisevar, average, predicted, predictedvar, observed, Kalman, corrected, correctedvar;

public:
  KalmanFilter(HCImage *img);
  void filter(HCImage *img);
};

#endif // KALMANNFILTER_H
