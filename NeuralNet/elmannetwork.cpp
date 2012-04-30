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

#include "elmannetwork.h"

using namespace NeuralNET;

ElmanNetwork::ElmanNetwork(unsigned layerCount, unsigned sizes[], unsigned dimension, float alpha)
    :NeuralNetwork(layerCount,sizes,dimension+sizes[0],alpha)
{
    hiddenSize = sizes[0];
    hiddenCopy.resize(hiddenSize,0);
}

void ElmanNetwork::resetCopy()
{
    hiddenCopy.resize(0);
    hiddenCopy.resize(hiddenSize,0);
}

void ElmanNetwork::updateCopy()
{
    hiddenCopy = tmpHiddenCopy;
}

vector<float> ElmanNetwork::classify(vector<float> input)
{
    input.insert(input.end(),hiddenCopy.begin(),hiddenCopy.end());
    tmpHiddenCopy = layers[0]->classify(input);
    return NeuralNetwork::classify(input);
}

float ElmanNetwork::train(vector<float> input, vector<int> target)
{
    input.insert(input.end(),hiddenCopy.begin(),hiddenCopy.end());
    tmpHiddenCopy = layers[0]->classify(input);
    return NeuralNetwork::train(input,target);
}
