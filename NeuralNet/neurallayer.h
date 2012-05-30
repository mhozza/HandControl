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
    vector<float> getDelta(vector<float> output, vector<int> target);
    virtual void trainDelta(vector<float> input,vector<float> delta);
    float derivativeFunction(float x, unsigned i);
    void setMomentum(float);
    void setW0(float);
  protected:
    NeuralLayer(){}
    float getError(vector<float>,vector<int>);
    vector<Perceptron*> neurons;
  public:
    NeuralLayer(unsigned size, unsigned dimension, float alpha);
    virtual float train(vector<float>,vector<int>);
    virtual vector<float> classify(vector<float>);
    virtual vector<int> discreteClassify(vector<float>);
  };
}

#endif // NEURALLAYER_H
