#ifndef CONTINUOUS_PERCEPTRON_H
#define CONTINUOUS_PERCEPTRON_H

#include "perceptron.h"
#include <cmath>

using namespace std;

namespace NeuralNET
{
  class ContinuousPerceptron : public Perceptron
  {
    protected:
      float activationFunction(const vector<float>*);
      float derivativeFunction(float);
    public:
      ContinuousPerceptron(unsigned dimension, float alpha = 0.25) : Perceptron(dimension,alpha) {  }

  };
}
#endif // CONTINUOUS_PERCEPTRON_H
