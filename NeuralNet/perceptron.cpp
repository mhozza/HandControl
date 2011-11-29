#include "perceptron.h"
using namespace NeuralNET;
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>

Perceptron::Perceptron(unsigned dimension, float alpha = 0.25) : momentum(0),  w0(0), alpha(alpha),  dimension(dimension+1)
{
  weights.resize(this->dimension,0);  
  deltaW.resize(this->dimension,0);
}

void Perceptron::prepare(vector<float>* input)
{
  if(input->size()==dimension-1) //bad input
  {    
    input->push_back(1);
  }
  else
  {
    throw -1;
  }
}

float Perceptron::getError(float output, int target)
{   
   return 0.5*(target-output)*(target-output);
}

float Perceptron::classify(vector<float> input)
{
  prepare(&input);
  return activationFunction(&input);  
}

void Perceptron::randomizeWeights()
{
  for(unsigned i = 0;i<dimension;i++)
  {
    weights[i]=(2*(float)rand()/RAND_MAX)-1;
  }    
}

void Perceptron::save(string filename)
{
  ofstream outfile(filename.c_str());
  for(unsigned i = 0;i<dimension;i++)
  {
    if(i>0) outfile << " ";
    outfile << weights[i];    
  }
  outfile << endl;
  outfile.close();  
}

void Perceptron::load(string filename)
{
  ifstream ifs ( filename.c_str() , ifstream::in );
  unsigned i = 0;
  while(i<dimension && ifs >> weights[i++]);
  ifs.close();
}
    
void Perceptron::printWeights()
{
  for(unsigned i = 0;i<dimension;i++)
  {
    if(i>0) cout << " ";
    cout << weights[i];    
  }
  cout << endl;
}

int Perceptron::discreteClassify(vector<float> input)
{
  return classify(input)>=.5;
}

void Perceptron::train(vector<float> input, int target)
{
  prepare(&input);
  float output = activationFunction(&input);
  trainDelta(&input,getDelta(output, target));
}

void Perceptron::trainDelta(vector<float> *input,float delta)
{
  for(unsigned j=0;j<dimension;j++)
  {    
    float m = momentum*deltaW[j];
    deltaW[j] = alpha*delta*(*input)[j] + m;
    weights[j]=weights[j]+ deltaW[j];
    if(abs(weights[j])<=w0)
    {
      weights[j]=0;
    }
    //weights[j]/=w0;
  }
}

void Perceptron::trainDelta(vector<float> input,float delta)
{
  prepare(&input);
  trainDelta(&input,delta);
}

float Perceptron::getDelta(float output, int target)
{
  return (target-output)*derivativeFunction(output);
}

float Perceptron::getDelta(vector<float> input, int target)
{
  prepare(&input);
  float output = activationFunction(&input);
  return getDelta(output,target);
}

vector<float> Perceptron::getWeights()
{
  return weights;
}

void Perceptron::setWeights(vector<float> w)
{
  weights = w;
}
