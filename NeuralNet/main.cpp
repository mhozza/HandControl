#include <iostream>
#include "neuralnetwork.h"
#include "distributedneuralnetwork.h"
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <csignal>
#include <cstdio>
#include <sys/types.h>
#include <dirent.h>


using namespace std;
using namespace NeuralNET;

#define FOR(i,n) for(int i=0;i<(n);i++)
#define N_SIDE 128
#define N (N_SIDE * N_SIDE)
#define HIDDEN_N 16
#define HIDDEN_N_W 4
#define HIDDEN_N2 5
#define OUT_N 1

inline void print(string s)
{
  cout << s << endl;
}

volatile int stop = 0;

void ctrlc(int signal __attribute__((unused)))
{
    stop = 1;
}

float getError(float output,bool target)
{
   return 0.5*(target-output)*(target-output);
}

vector<string> listDirectory(string path)
{
   vector<string> files;
   if(path[path.size()-1]!='/') path+='/';
   DIR *dp;
   struct dirent *ep;

   dp = opendir(path.c_str());
   if (dp != NULL)
   {
     while ((ep = readdir (dp)))
     {
       string s = ep->d_name;
       if(s=="." || s == "..") continue;
       files.push_back(path+s);
     }
     closedir(dp);
   }
   else
     perror ("Couldn't open the directory");
   return files;
}

template<class T> vector<T> make_vector(T t)
{
  vector<T> vt;
  vt.push_back(t);
  return vt;
}

vector<float> loadImage(string path)
{
  print(path);
  ifstream ifs(path.c_str());
  vector<float> res;
  res.resize(N,0);
/*
  //ppm without comments
  //head
  string hdr = "P2";
  ifs >> hdr;
  if(hdr!="P2") throw 1;
  //w, h
  int w = N_SIDE,h=N_SIDE;
  int colors = 256;
  ifs >> w >> h >> colors;
  if(w!=N_SIDE || h!=N_SIDE) throw 1;*/

  FOR(i,N)
  {
      //int a;
      //ifs >> a;
      //res[i] = 1/(1+a);
      ifs >> res[i];
  }
  ifs.close();
  return res;
}

int main(int argc, char *argv[])
{
   srand (time(NULL) );
   float alpha = 0.25;
   int verbose = 0;
   int mode = 0;
   if(argc>1) mode = atoi(argv[1]);

   //unsigned sizes[] = {HIDDEN_N, OUT_N};
   //NeuralNetwork *net = new NeuralNetwork(2,sizes,N,alpha);
   unsigned sizes[] = {HIDDEN_N, HIDDEN_N2, OUT_N};
   NeuralNetwork *net = new DistributedNeuralNetwork(3,sizes,HIDDEN_N_W,N_SIDE,N_SIDE,alpha);
   signal(SIGINT, ctrlc);

   vector<pair<vector<float>,vector<int > > > tests;

   //net->saveWeights("blabla");

   string hands_path, nonhands_path;
   if(mode==0)
   {
    hands_path = "../hand_images/hands";
    nonhands_path = "../hand_images/other";
    net->loadWeights("vahy");
   }
   else
   {
     hands_path = "../hand_images/hands";
     nonhands_path = "../hand_images/other";
    //hands_path = "../hand_images/test/hands";
    //nonhands_path = "../hand_images/test/other";
    net->loadWeights("vahy");
   }


   //nacitaj
   //net->loadWeights("vahy");
   vector<string> hands = listDirectory(hands_path);
   vector<string> nonhands = listDirectory(nonhands_path);

   for(unsigned i = 0;i<hands.size();i++)
   {
     tests.push_back(make_pair(loadImage(hands[i]),make_vector(1)));
   }

   for(unsigned i = 0;i<nonhands.size();i++)
   {
     tests.push_back(make_pair(loadImage(nonhands[i]),make_vector(0)));
   }

   float E = 100;
   int epoche = 0;
   while(epoche<400 && (mode==0 || epoche<1))
   {
     epoche++;
     cerr << epoche << endl;
     E=0;
     if(mode==0)
        random_shuffle(tests.begin(),tests.end());

     FOR(i,tests.size())
     {
        if(verbose)
        {
           cout << i << ": ";
           cout << tests[i].second[0] << " ";
        }
        vector<float> tc = net->classify(tests[i].first);
       float c = tc[0];
       if(verbose)
        cout << (c>0.5) << " (" << c  << ")" << endl;

       float e = 0;
       if(mode==0)
         e = net->train(tests[i].first,tests[i].second);
       else
         e = getError(c,tests[i].second[0]);

       E += e;
     }

     cout << E << endl;
     if(stop) break;
   }
   if(verbose)cout << epoche << endl;
   if(mode == 0) net->saveWeights("vahy");

}
