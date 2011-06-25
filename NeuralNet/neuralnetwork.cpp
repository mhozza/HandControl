#include "neuralnetwork.h"

namespace NeuralNET
{

  NeuralNetwork::NeuralNetwork(unsigned layerCount, unsigned sizes[], unsigned dimension, float alpha = .25)
  {
    if(layerCount>sizeof(sizes))
    {
      throw -1;
    }

    layers.resize(layerCount);
    for(unsigned i = 0; i< layers.size();i++)
    {      
      layers[i] = new NeuralLayer(sizes[i],i==0 ? dimension : sizes [i-1],alpha);
      //layers[i]->setW0(0.01);
      //layers[i]->setMomentum(0.3);

    }
    //layers[0]->setW0(0.01);
    //layers.back()->setMomentum(0.25);


  }

  float NeuralNetwork::train(vector<float> input,vector<int> target)
  {
    vector<vector<float> > in;
    in.resize(layers.size()+1);
    in[0] = input;

    for(unsigned i = 0;i<layers.size();i++)
    {
      in[i+1] = layers[i]->classify(in[i]);
    }

    float err = layers.back()->getError(in.back(),target);
    if(err>0)
    {
      vector<vector<float> > deltas;
      deltas.resize(layers.size());
      deltas.back() = layers.back()->getDelta(in.back(),target);
      for(int i = layers.size()-2;i>=0;i--)
      {
        vector<vector<float> > weights = switchIndex(layers[i+1]->getWeights());
        for(unsigned j = 0;j<weights.size()-1;j++)
        {
           deltas[i].push_back( weightSum(deltas[i+1].begin(), deltas[i+1].end(), weights[j].begin(), weights[j].end() )*layers[i]->derivativeFunction(in[i+1][j],j));
        }
      }

      for(unsigned i = 0;i<layers.size();i++)
      {
        NeuralLayer *l = layers[i];
        l->trainDelta(in[i],deltas[i]);
      }
    }

    return err;
  }

  vector<float> NeuralNetwork::classify(vector<float> input)
  {
    vector<float> in = input;

    for(unsigned i = 0;i<layers.size()-1;i++)
    {
      in = layers[i]->classify(in);
    }
    return layers[layers.size()-1]->classify(in);
  }

  float NeuralNetwork::classify1(vector<float> input)
  {
    vector<float> t = classify(input);
    return t[0];
  }

  vector<int> NeuralNetwork::discreteClassify(vector<float> input)
  {
    vector<float> o = classify(input);
    vector<int> r;
    r.resize(o.size());

    for(unsigned i = 0;i<o.size();i++)
    {
      r[i]=o[i]>.5;
    }
    return r;
  }

  void NeuralNetwork::saveWeights(string fname)
  {
    ofstream ofs(fname.c_str());
    for(unsigned i = 0;i<layers.size();i++)
    {
      vector<vector<float> > w = layers[i]->getWeights();
      for(unsigned j = 0;j<w.size();j++)
      {
        for(unsigned k = 0;k<w[j].size();k++)
        {
          ofs << w[j][k] << " ";
        }
        ofs << endl;
      }
      ofs << endl;
    }
    ofs.close();
  }

  void NeuralNetwork::loadWeights(string fname)
  {
    ifstream ifs(fname.c_str());
    for(unsigned i = 0;i<layers.size();i++)
    {
      vector<vector<float> > w = layers[i]->getWeights();
      for(unsigned j = 0;j<w.size();j++)
      {
        for(unsigned k = 0;k<w[j].size();k++)
        {
          ifs >> w[j][k];
        }
      }
      layers[i]->setWeights(w);
    }
  }

}
