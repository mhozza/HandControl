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

#include "kalmanfilter.h"

KalmanFilter::KalmannFilter(HCImage *img)
  :percentvar(0.05), gain(0.8), width(img->width()), height(img->height())
{
  /*dimension = width*height;
  noisevar.resize(dimension,percentvar);
  predicted.resize(dimension,0);
  predictedvar.resize(dimension,percentvar);
  observed.resize(dimension,0);
  Kalman.resize(dimension,0);
  corrected.resize(dimension,0);
  correctedvar.resize(dimension,0);

  //firstImage
  double* d = img->toDoubleArray();
  for(int i=0;i<predicted.size();++i)
    predicted[i] = d[i];*/
}


void KalmanFilter::filter(HCImage *img)
{
  //observed = img->toDoubleArray();
  //observed.insert(observed.begin(),dimension,img->toDoubleArray());
  /*double* d = img->toDoubleArray();
  for(int i=0;i<observed.size();++i)
    observed[i] = d[i];

  for(int k=0;k<Kalman.size();++k)
    Kalman[k] = predictedvar[k]/(predictedvar[k]+noisevar[k]);
  for(int k=0;k<corrected.size();++k)
    corrected[k] = gain*predicted[k]+(1.0-gain)*observed[k]+Kalman[k]*(observed[k] - predicted[k]);
  for(int k=0;k<correctedvar.size();++k)
    correctedvar[k] = predictedvar[k]*(1.0 - Kalman[k]);
  predictedvar = correctedvar;
  predicted = corrected;
  //save Image
  ImageBuffer b;  b.resize(corrected.size());
  for(int i=0;i<corrected.size();++i)
    b[i] = (uchar)corrected[i];
  img->setImage(b,width,height);*/
}
