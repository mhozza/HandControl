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

#ifndef DISTRIBUTEDRECURRENTNETWORK_H
#define DISTRIBUTEDRECURRENTNETWORK_H

#include "recurrentnetwork.h"

namespace NeuralNET
{

class DistributedRecurrentNetwork : public RecurrentNetwork
{
public:
    DistributedRecurrentNetwork(unsigned layerCount, unsigned sizes[], unsigned firstLayerW, unsigned firstLayerH, unsigned dimensionW, unsigned dimensionH, float alpha);
};

}

#endif // DISTRIBUTEDRECURRENTNETWORK_H
