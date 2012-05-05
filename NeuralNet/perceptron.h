#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <vector>
#include <string>

using namespace std;

namespace NeuralNET
{

  template <class InputIterator, class InputIterator2>
    float weightSum ( InputIterator first, InputIterator last, InputIterator2 first2, InputIterator2 last2 )
  {
    float ret=0.0;
    while (first != last && first2!=last2) ret+=(*first++)*(*first2++);
    return ret;
  }

  class Perceptron
  {
    friend class NeuralLayer;
    void trainDelta(vector<float>*,float);
    virtual float activationFunction(const vector<float>* input) = 0;
    virtual float derivativeFunction(float x) = 0;
    float getDelta(vector<float>,int);
    float momentum;
    float w0;
    vector<float> deltaW;
  protected:
    virtual void prepare(vector<float>* input);
    float alpha;
    unsigned dimension;
    vector<float> weights;    
  public:
    Perceptron(unsigned, float);    

    void train(vector<float> input,int target);
    void trainDelta(vector<float> input, float delta);

    virtual float classify(vector<float> input);
    int discreteClassify(vector<float> input);

    void randomizeWeights();
    void save(string filename);
    void load(string filename);
    void printWeights();
    vector<float> getWeights();
    void setWeights(vector<float>);
    float getDelta(float,int);
    float getError(float, int);
    //void setMomentum(bool);
  };
}

#endif // PERCEPTRON_H

