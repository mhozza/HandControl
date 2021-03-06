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

#ifndef RECURRENTLAYER_H
#define RECURRENTLAYER_H

#include "neurallayer.h"
#include "recurrentperceptron.h"

namespace NeuralNET
{

class RecurrentLayer : virtual public NeuralLayer
{   
public:
    RecurrentLayer(unsigned size, unsigned dimension, float alpha);
    void update();
    void reset();
};

}

#endif // RECURRENTLAYER_H
