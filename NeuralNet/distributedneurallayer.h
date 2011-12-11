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

#ifndef DISTRIBUTEDNEURALLAYER_H
#define DISTRIBUTEDNEURALLAYER_H

#include "neurallayer.h"
namespace NeuralNET
{
  class DistributedNeuralLayer : public NeuralLayer
  {
    unsigned neuronsPerZone, width, height, dimensionWidth, dimensionHeight, inputWidth, inputHeight;
    vector<float> prepareInput(unsigned x, unsigned y, vector<float> &input);
    void trainDelta(vector<float> input,vector<float> delta);
  public:
      DistributedNeuralLayer(unsigned neuronsPerZone, unsigned w, unsigned h, unsigned dimensionW, unsigned dimensionH, float alpha);
      float train(vector<float>,vector<int>);
      vector<float> classify(vector<float>);
      vector<int> discreteClassify(vector<float>);
  };
}
#endif // DISTRIBUTEDNEURALLAYER_H
