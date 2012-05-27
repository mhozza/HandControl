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

#ifndef RECURRENTNETWORK_H
#define RECURRENTNETWORK_H

#include "neuralnetwork.h"
#include "recurrentlayer.h"

namespace NeuralNET {

class RecurrentNetwork : public NeuralNetwork
{
protected:
    unsigned recurrentLayerIndex;
    RecurrentNetwork() {}
public:
    RecurrentNetwork(unsigned layerCount, unsigned sizes[], unsigned dimension, float alpha = 0.25);
    void update();
    void reset();
};

}

#endif // RECURRENTNETWORK_H
