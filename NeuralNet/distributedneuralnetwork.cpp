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

#include "distributedneuralnetwork.h"
#include "distributedneurallayer.h"
using namespace NeuralNET;

#include <iostream>


DistributedNeuralNetwork::DistributedNeuralNetwork(unsigned layerCount, unsigned sizes[], unsigned firstLayerW, unsigned dimensionW, unsigned dimensionH, float alpha)
{
  /*if(layerCount>sizeof(sizes)/sizeof(unsigned))
  {
    std::cout << layerCount << " " << sizeof(sizes) << std::endl;
    throw -1;
  }*/

  layers.resize(layerCount);
  for(unsigned i = 0; i< layers.size();i++)
  {
    if(i==0)
    {
      layers[i] = new DistributedNeuralLayer(firstLayerW, sizes[i]/firstLayerW, dimensionW, dimensionH, alpha);
    }
    else
    {
      layers[i] = new NeuralLayer(sizes[i], sizes[i-1], alpha);
    }
  }
}
