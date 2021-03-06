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

#include "recurrentnetwork.h"

using namespace NeuralNET;

RecurrentNetwork::RecurrentNetwork(unsigned layerCount, unsigned sizes[], unsigned dimension, float alpha, unsigned recurrentLayerIndex)
    :recurrentLayerIndex(recurrentLayerIndex)
{
    layers.resize(layerCount);
    for(unsigned i = 0; i< layers.size();i++)
    {
      if(i==recurrentLayerIndex)
      {
        layers[i] = new RecurrentLayer(sizes[i], i==0 ? dimension : sizes [i-1], alpha);
      }
      else
      {
        layers[i] = new NeuralLayer(sizes[i], i==0 ? dimension : sizes [i-1], alpha);
      }
    }
}

void RecurrentNetwork::update()
{
    dynamic_cast<RecurrentLayer*>(layers[recurrentLayerIndex])->update();
}

void RecurrentNetwork::reset()
{
    dynamic_cast<RecurrentLayer*>(layers[recurrentLayerIndex])->reset();
}
