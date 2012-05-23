#include "continuous_perceptron.h"

using namespace NeuralNET;

#define MATH_E 2.7182818284590451


float ContinuousPerceptron::derivativeFunction(float x)
{
  //return x*(1-x);
  return pow(MATH_E,x)/pow((pow(MATH_E,x)+1),2);
}

float ContinuousPerceptron::activationFunction(const vector<float>* input)
{
  float sum = weightSum(input->begin(),input->end(),weights.begin(),weights.end());
  return (1./(1.+pow(MATH_E,-sum)));
}

