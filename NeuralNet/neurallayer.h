#ifndef NEURALLAYER_H
#define NEURALLAYER_H

#include "continuous_perceptron.h"

namespace NeuralNET
{

  class NeuralLayer
  {
    friend class NeuralNetwork;
    vector<vector<float> > getWeights();
    void setWeights(vector<vector<float> >);
    //vector<float> getDelta(vector<float> input,vector<int> target);
    vector<float> getDelta(vector<float> output, vector<int> target);
    float getError(vector<float>,vector<int>);
    void trainDelta(vector<float> input,vector<float> delta);
    float derivativeFunction(float x, int i);
    void setMomentum(float);
    void setW0(float);
  protected:
    vector<Perceptron*> neurons;
  public:
      NeuralLayer(unsigned size, unsigned dimension, float alpha);
      float train(vector<float>,vector<int>);
      vector<float> classify(vector<float>);
      vector<int> discreteClassify(vector<float>);
  };
}

#endif // NEURALLAYER_H
