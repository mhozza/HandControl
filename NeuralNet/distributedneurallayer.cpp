/*
 *    Copyright (C) 2011-2011  Michal Hozza (mhozza@gmail.com)
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

#include "distributedneurallayer.h"
using namespace NeuralNET;

DistributedNeuralLayer::DistributedNeuralLayer(unsigned w, unsigned h, unsigned dimensionW, unsigned dimensionH, float alpha)
  : NeuralLayer(w*h,(dimensionW*dimensionH)/(w*h),alpha),
    width(w), height(h), dimensionWidth(dimensionW), dimensionHeight(dimensionH),
    inputWidth(dimensionWidth/width), inputHeight(dimensionHeight/height)
{

}

vector<float> DistributedNeuralLayer::prepareInput(unsigned x, unsigned y, vector<float> &input)
{
  unsigned sx = x*inputWidth,sy = y*inputHeight;
  vector<float> in;
  for(unsigned iy = sy; iy < sy+inputHeight; iy++)
  {
    for(unsigned ix = sx; x < sx+inputWidth; ix++)
    {
      in.push_back(input[ix+iy*inputWidth]);
    }
  }
  return in;
}

vector<float> DistributedNeuralLayer::classify(vector<float> input)
{
  vector<float> outputs;
  outputs.resize(neurons.size());
  for(unsigned y = 0; y < height; y++)
  {
    for(unsigned x = 0; x < width; x++)
    {
      outputs[x+y*width] = neurons[x+y*width]->classify(prepareInput(x,y,input));
    }
  }
  return outputs;
}

vector<int> DistributedNeuralLayer::discreteClassify(vector<float> input)
{
  vector<int> outputs;
  outputs.resize(neurons.size());
  for(unsigned y = 0; y < height; y++)
  {
    for(unsigned x = 0; x < width; x++)
    {
      outputs[x+y*width] = neurons[x+y*width]->discreteClassify(prepareInput(x,y,input));
    }
  }
  return outputs;
}


float DistributedNeuralLayer::train(vector<float> input,vector<int> target)
{

  float e = getError(classify(input),target);
  if(e>0)
  {
    for(unsigned y = 0; y < height; y++)
    {
      for(unsigned x = 0; x < width; x++)
      {
        neurons[x+y*width]->train(prepareInput(x,y,input),target[x+y*width]);
      }
    }
  }
  return e;
}

void DistributedNeuralLayer::trainDelta(vector<float> input,vector<float> delta)
{
  for(unsigned y = 0; y < height; y++)
  {
    for(unsigned x = 0; x < width; x++)
    {
       neurons[x+y*width]->trainDelta(prepareInput(x,y,input), delta[x+y*width]);
    }
  }
}
