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

#ifndef RECURENTPERCEPTRON_H
#define RECURENTPERCEPTRON_H

#include "continuous_perceptron.h"

namespace NeuralNET
{

class RecurentPerceptron : public ContinuousPerceptron
{
    float lastOutput, tmpLastOutput;
public:
    RecurentPerceptron(unsigned int dimension, float alpha);
    float getLastOutput();
    void updateLastOutput();

    //void train(vector<float> input, int target);
    //void trainDelta(vector<float> input, float target);

    float classify(vector<float> input);
    //int discreteClassify(vector<float> input);
};

}
#endif // RECURENTPERCEPTRON_H
