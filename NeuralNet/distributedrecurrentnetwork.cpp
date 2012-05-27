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

#include "distributedrecurrentnetwork.h"
#include "distributedrecurrentlayer.h"

using namespace NeuralNET;

DistributedRecurrentNetwork::DistributedRecurrentNetwork(unsigned layerCount, unsigned sizes[], unsigned firstLayerW, unsigned firstLayerH, unsigned dimensionW, unsigned dimensionH, float alpha)
    :RecurrentNetwork(0)
{
    layers.resize(layerCount);
    for(unsigned i = 0; i< layers.size();i++)
    {
      if(i==0)
      {
        layers[i] = new DistributedRecurrentLayer(sizes[i]/(firstLayerW*firstLayerH),firstLayerW,firstLayerH , dimensionW, dimensionH, alpha);
      }
      else
      {
        layers[i] = new NeuralLayer(sizes[i], sizes[i-1], alpha);
      }
    }
}
