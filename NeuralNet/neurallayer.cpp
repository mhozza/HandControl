#include "neurallayer.h"


using namespace std;
using namespace NeuralNET;

NeuralLayer::NeuralLayer(unsigned size, unsigned dimension, float alpha = 0.25)
{
  neurons.resize(size);
  for(unsigned i = 0; i< neurons.size();i++)
  {
    //neurons[i] = new BinaryPerceptron(dimension,alpha);
    neurons[i] = new ContinuousPerceptron(dimension,alpha);
    neurons[i]->randomizeWeights();
  }
}

float NeuralLayer::train(vector<float> input,vector<int> target)
{
  float e = getError(classify(input),target);
  if(e>0)
  {
    for(unsigned i = 0; i< neurons.size();i++)
    {
      neurons[i]->train(input,target[i]);
    }
  }
  return e;
}

void NeuralLayer::trainDelta(vector<float> input,vector<float> delta)
{
  for(unsigned i = 0; i< neurons.size();i++)
  {
    neurons[i]->trainDelta(input, delta[i]);
  }
}

vector<float> NeuralLayer::classify(vector<float> input)
{
  vector<float> outputs;  
  outputs.resize(neurons.size());
  for(unsigned i = 0; i< neurons.size();i++)
  {
    outputs[i] = neurons[i]->classify(input);   
  }  
  return outputs;
}

vector<int> NeuralLayer::discreteClassify(vector<float> input)
{
  vector<int> out;  
  out.resize(neurons.size());  
  for(unsigned i = 0; i< neurons.size();i++)
  {
    out[i] = neurons[i]->discreteClassify(input);    
  }  
  return out;
}

vector<vector<float> > NeuralLayer::getWeights()
{
  vector<vector<float> > weights;
  weights.resize(neurons.size());
  for(unsigned i = 0; i< neurons.size();i++)
  {
    weights[i] = neurons[i]->getWeights();
  }
  return weights;
}

void NeuralLayer::setWeights(vector<vector<float> > w)
{
  for(unsigned i = 0; i< neurons.size();i++)
  {
    neurons[i]->setWeights(w[i]);
  }
}


/*vector<float> NeuralLayer::getDelta(vector<float> input,vector<int> target)
{
  vector<float> deltas;
  deltas.resize(neurons.size());
  for(unsigned i = 0; i< neurons.size();i++)
  {
    deltas[i] = neurons[i]->getDelta(input,target[i]);
  }
  return deltas;
}*/

vector<float> NeuralLayer::getDelta(vector<float> output,vector<int> target)
{
  vector<float> deltas;
  deltas.resize(neurons.size());
  for(unsigned i = 0; i< neurons.size();i++)
  {
    deltas[i] = neurons[i]->getDelta(output[i],target[i]);
  }
  return deltas;
}

float NeuralLayer::getError(vector<float> output,vector<int> target)
{
  float error = 0;
  for(unsigned i = 0; i< neurons.size();i++)
  {
    error += neurons[i]->getError(output[i],target[i]);
  }
  return error;
}

float NeuralLayer::derivativeFunction(float x, int i = 0)
{
  return neurons[i]->derivativeFunction(x);
}

void NeuralLayer::setMomentum(float m)
{
  for(unsigned i = 0; i< neurons.size();i++)
  {
    neurons[i]->momentum = m;
  }
}

void NeuralLayer::setW0(float w)
{
  for(unsigned i = 0; i< neurons.size();i++)
  {
    neurons[i]->w0 = w;
  }
}
