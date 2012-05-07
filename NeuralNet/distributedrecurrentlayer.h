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

#ifndef DISTRIBUTEDRECURRENTLAYER_H
#define DISTRIBUTEDRECURRENTLAYER_H

#include "recurrentlayer.h"
#include "distributedneurallayer.h"

namespace NeuralNET {

class DistributedRecurrentLayer : public RecurrentLayer, public DistributedNeuralLayer
{
public:
    DistributedRecurrentLayer(unsigned neuronsPerZone, unsigned w, unsigned h, unsigned dimensionW, unsigned dimensionH, float alpha);
};

}

#endif // DISTRIBUTEDRECURRENTLAYER_H
