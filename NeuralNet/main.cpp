#include <iostream>
#include "neuralnetwork.h"
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <csignal>
#include <cstdio>
#include <sys/types.h>
#include <dirent.h>


using namespace std;
using namespace NeuralNET;

#define FOR(i,n)      for(int i=0;i<(n);i++)
#define N 6400
#define N_SIDE 80
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
     while (ep = readdir (dp))
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
  res.resize(N);
  //ppm without comments
  //head
  string hdr;
  ifs >> hdr;
  if(hdr!="P1") throw 1;
  //w, h
  int w,h;
  ifs >> w >> h;
  if(w!=N_SIDE || h!=N_SIDE) throw 1;
  int i = 0;
  FOR(y,h)
  {
    FOR(x,w)
    {
      int t;
      ifs >> t;
      res[i++] = t;
    }
  }
  ifs.close();
  return res;
}

int main(int argc, char *argv[])
{
   srand (time(NULL) );
   float alpha = 0.2;
   int verbose = 10;
   int mode = 0;
   if(argc>1) mode = atoi(argv[1]);

   unsigned sizes[] = {12,OUT_N};
   NeuralNetwork *net = new NeuralNetwork(2,sizes,N,alpha);
   signal(SIGINT, ctrlc);

   vector<pair<vector<float>,vector<int > > > tests;

   string hands_path, nonhands_path;
   if(mode==0)
   {
    hands_path = "../hand_images/hands";
    nonhands_path = "../hand_images/other";
   }
   else
   {
    hands_path = "../hand_images/test/hands";
    nonhands_path = "../hand_images/test/other";
    net->loadWeights("vahy");
   }

   //nacitaj
   vector<string> hands = listDirectory(hands_path);
   vector<string> nonhands = listDirectory(nonhands_path);

   for(int i = 0;i<hands.size();i++)
   {
     tests.push_back(make_pair(loadImage(hands[i]),make_vector(1)));
   }

   for(int i = 0;i<nonhands.size();i++)
   {
     tests.push_back(make_pair(loadImage(nonhands[i]),make_vector(0)));
   }

   float E = 100;
   int epoche = 0;
   while(epoche<200 && (mode==0 || epoche<1))
   {
     epoche++;
     cerr << epoche << endl;
     E=0;
     random_shuffle(tests.begin(),tests.end());

     FOR(i,tests.size())
     {
       cout << i << ": ";
       cout << tests[i].second[0] << " ";
       vector<float> tc = net->classify(tests[i].first);
       float c = tc[0];
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
