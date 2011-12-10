#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <fstream>

#include "neurallayer.h"

namespace NeuralNET
{
  template <class T> vector<vector<T> > switchIndex ( vector<vector<T> > v )
  {
    vector<vector<T> > v2;
    v2.resize(v[0].size());
    for(unsigned i = 0;i<v.size();i++)
    {
      for(unsigned j = 0;j<v[i].size();j++)
      {
        v2[j].resize(v.size());
        v2[j][i] = v[i][j];
      }
    }
    return v2;
  }

  class NeuralNetwork
  {    
  protected:
    NeuralNetwork();
    vector<NeuralLayer*> layers;
  public:
    NeuralNetwork(unsigned layerCount, unsigned sizes[], unsigned dimension, float alpha);
    float train(vector<float>,vector<int>);
    vector<float> classify(vector<float> input);
    float classify1(vector<float> input);
    vector<int> discreteClassify(vector<float> input);
    void saveWeights(string fname);
    void loadWeights(string fname);
  };
}

#endif // NEURALNETWORK_H
