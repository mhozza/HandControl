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

#ifndef ELMANNETWORK_H
#define ELMANNETWORK_H

#include "neuralnetwork.h"

namespace NeuralNET {

class ElmanNetwork : public NeuralNetwork
{
    vector<float> hiddenCopy, tmpHiddenCopy;
    unsigned hiddenSize;
public:
    ElmanNetwork(unsigned layerCount, unsigned sizes[], unsigned dimension, float alpha);
    void updateCopy();
    void resetCopy();
    float train(vector<float> input,vector<int> target);
    vector<float> classify(vector<float> input);
};

}

#endif // ELMANNETWORK_H
