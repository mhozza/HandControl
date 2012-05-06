#include <iostream>
#include "neuralnetwork.h"
#include "distributedneuralnetwork.h"
#include "recurrentnetwork.h"
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <csignal>
#include <cstdio>
#include <sys/types.h>
#include <dirent.h>
#include <map>


using namespace std;
using namespace NeuralNET;

#define FOR(i,n) for(int i=0;i<(n);i++)
#define N_SIDE 128
#define N (N_SIDE * N_SIDE)
#define HIDDEN_N_SIDE 4
#define HIDDEN_N HIDDEN_N_SIDE*HIDDEN_N_SIDE*3
#define HIDDEN_N2 11
#define OUT_N 1

struct IndexInfo
{
    uint seqIndex, frameIndex;
    char partIndex;
    IndexInfo(uint seqIndex,uint frameIndex, char partIndex):seqIndex(seqIndex),
        frameIndex(frameIndex), partIndex(partIndex){}
};

string baseName(string path)
{
    size_t pos  = path.rfind('/');
    if(pos==string::npos) return path;
    return path.substr(pos+1);
}

IndexInfo parseFilename(string path, uint prefixLength = 4, uint suffixLength = 4)
{    
    path = baseName(path);
    string s = path.substr(prefixLength,path.size()-prefixLength-suffixLength);
    char ch = s[s.size()-1];
    size_t pos  = s.find('_');
    string s1 = s.substr(0,pos);
    string s2 = s.substr(pos+1,s.size()-pos-2);
    //cerr << s1 << " " << s2 << " " << ch << endl;
    return IndexInfo(atoi(s1.c_str()), atoi(s2.c_str()), ch);
}

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
       if(s=="." || s == ".." || s=="nu") continue;
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

vector<float> loadImage(string path, int datatype = 0, bool invert = false)
{
  print(path);
  ifstream ifs(path.c_str());
  vector<float> res;
  res.resize(N,0);
  if(datatype == 1)
  {
    //ppm without comments
    //head
    string hdr = "P2";
    ifs >> hdr;
    if(hdr!="P2") throw 1;
    //w, h
    int w = N_SIDE,h=N_SIDE;
    int colors = 256;
    ifs >> w >> h >> colors;
    if(w!=N_SIDE || h!=N_SIDE) throw 1;
  }

  FOR(i,N)
  {
      if(datatype==1)
      {
          int a;
          ifs >> a;
          if(invert)
          {
              a = 255-a;
          }
          res[i] = 1/(1+a);
      }
      else
      {
          ifs >> res[i];
      }
  }
  ifs.close();
  return res;
}

map<unsigned,unsigned> indexMap;
unsigned currIndexMap = 0;

unsigned getIndex(unsigned seqIndex)
{
    if(indexMap.find(seqIndex)==indexMap.end())
    {
        indexMap[seqIndex] = currIndexMap++;
    }
    return indexMap[seqIndex];
}

int recurrentTrain(int argc, char *argv[], int param_offest = 0)
{
   srand(time(NULL)+42);

   float alpha = 0.2;
   int verbose = 0;
   int mode = 0;
   int datatype = 0;//0 - float data, 1 - image
   bool invert = false;

   if(argc>1+param_offest) mode = atoi(argv[1+param_offest]);
   if(argc>2+param_offest) verbose = atoi(argv[2+param_offest]);
   if(argc>3+param_offest) datatype = atoi(argv[3+param_offest]);
   if(datatype==3)
   {
       datatype =1;
       invert = true;
   }

   string hands_path = "", nonhands_path = "";
   if(argc>4+param_offest) hands_path = argv[4+param_offest];
   if(argc>5+param_offest) nonhands_path = argv[5+param_offest];
   string infile = "vahy";
   if(argc>6+param_offest) infile = argv[6+param_offest];
   string outfile = infile;
   if(argc>7+param_offest) outfile = argv[7+param_offest];


   unsigned sizes[] = {HIDDEN_N, OUT_N};
   RecurrentNetwork *net = new RecurrentNetwork(2,sizes,N,alpha);
   //unsigned sizes[] = {HIDDEN_N, HIDDEN_N2, OUT_N};
   //RecurrentNetwork *net = new RecurrentNetwork(3,sizes,HIDDEN_N_SIDE, HIDDEN_N_SIDE, N_SIDE,N_SIDE,alpha);
   signal(SIGINT, ctrlc);

   //vector<pair<vector<float>,vector<int > > > tests;
   vector<vector<vector<pair<vector<float>,vector<int > > > > > tests;//tests[seqIndex][frame][part]


   //net->saveWeights("blabla");

   if(mode==0)
   {
     if(hands_path=="")
       hands_path = "../hand_images/hands";
     if(nonhands_path=="")
       nonhands_path = "../hand_images/other";
     net->loadWeights(infile);
   }
   else
   {
    // hands_path = "../hand_images/hands";
    // nonhands_path = "../hand_images/other";
    if(hands_path=="")
      hands_path = "../hand_images/test/hands";
    if(nonhands_path=="")
      nonhands_path = "../hand_images/test/other";
    net->loadWeights(infile);
   }


   //nacitaj
   //net->loadWeights(infile);
   vector<string> hands = listDirectory(hands_path);
   vector<string> others = listDirectory(nonhands_path);

   for(unsigned i = 0;i<hands.size();i++)
   {
       IndexInfo ii = parseFilename(hands[i]);

       unsigned si = getIndex(ii.seqIndex);
       if(si>=tests.size())
       {
           tests.resize(si+1);
       }
       if(ii.frameIndex>=tests[si].size())
       {
           tests[si].resize(ii.frameIndex+1);
       }
       tests[si][ii.frameIndex].push_back(make_pair(loadImage(hands[i],datatype,invert),make_vector(1)));
       //tests.push_back(make_pair(loadImage(hands[i],datatype,invert),make_vector(1)));
   }

   for(unsigned i = 0;i<others.size();i++)
   {
       IndexInfo ii = parseFilename(others[i]);

       unsigned si = getIndex(ii.seqIndex);
       if(si>=tests.size())
       {
           tests.resize(tests.size()+1);
       }
       if(ii.frameIndex>=tests[si].size())
       {
           tests[si].resize(tests[si].size()+1);
       }
       tests[si][ii.frameIndex].push_back(make_pair(loadImage(others[i],datatype,invert),make_vector(0)));
     //tests.push_back(make_pair(loadImage(nonhands[i],datatype,invert),make_vector(0)));
   }

   float E = 100;
   int epoche = 0;
   int good = 0;
   while(epoche<400 && (mode==0 || epoche<1))
   {
     epoche++;
     if(mode==0) cerr << epoche << endl;
     E=0;
     if(mode==0)
        random_shuffle(tests.begin(),tests.end());

     FOR(i,tests.size())
     {
         FOR(j,tests[i].size())
         {
             FOR(k,tests[i][j].size())
             {
                 if(verbose)
                 {
                     cout << i << ": ";
                     cout << tests[i][j][k].second[0] << " ";
                 }
                 vector<float> tc = net->classify(tests[i][j][k].first);
                 float c = tc[0];
                 if(verbose)
                     cout << (c>0.5) << " (" << c  << ")" << endl;


                 float e = 0;
                 if(mode==0)
                     e = net->train(tests[i][j][k].first,tests[i][j][k].second);
                 else
                 {
                     if(abs(tests[i][j][k].second[0] - c)<0.5) good++;
                     e = getError(c,tests[i][j][k].second[0]);
                     cout << e << endl;
                 }
                 if(tests[i][j][k].second[0]==1)
                 {
                     //net->update();
                     cerr << "Update" << endl;
                 }
                 E += e;
             }
         }
     }
     cout << "Final error:" << E << endl;
     if(mode>0)
     {
         cout << "Good: " << good << " of " << hands.size()+others.size() << " "
              << (100.0*good)/(hands.size()+others.size()) << "%" << endl;
     }
     if(stop) break;
   }
   if(verbose)cout << epoche << endl;
   if(mode == 0) net->saveWeights(outfile);
   return 0;
}

int normalTrain(int argc, char *argv[], int param_offest = 0)
{
   srand(time(NULL)+42);

   float alpha = 0.2;
   int verbose = 0;
   int mode = 0;
   int datatype = 0;//0 - float data, 1 - image
   bool invert = false;

   if(argc>1+param_offest) mode = atoi(argv[1+param_offest]);
   if(argc>2+param_offest) verbose = atoi(argv[2+param_offest]);
   if(argc>3+param_offest) datatype = atoi(argv[3+param_offest]);
   if(datatype==3)
   {
       datatype =1;
       invert = true;
   }

   string hands_path = "", nonhands_path = "";
   if(argc>4+param_offest) hands_path = argv[4+param_offest];
   if(argc>5+param_offest) nonhands_path = argv[5+param_offest];
   string infile = "vahy";
   if(argc>6+param_offest) infile = argv[6+param_offest];
   string outfile = infile;
   if(argc>7+param_offest) outfile = argv[7+param_offest];

   //unsigned sizes[] = {HIDDEN_N, OUT_N};
   unsigned sizes[] = {HIDDEN_N, HIDDEN_N2, OUT_N};
   //NeuralNetwork *net = new NeuralNetwork(2,sizes,N,alpha);
   NeuralNetwork *net = new DistributedNeuralNetwork(3,sizes,HIDDEN_N_SIDE, HIDDEN_N_SIDE, N_SIDE,N_SIDE,alpha);
   signal(SIGINT, ctrlc);

   vector<pair<vector<float>,vector<int > > > tests;

   //net->saveWeights("blabla");

   if(mode==0)
   {
     if(hands_path=="")
       hands_path = "../hand_images/hands";
     if(nonhands_path=="")
       nonhands_path = "../hand_images/other";
     net->loadWeights(infile);
   }
   else
   {
    // hands_path = "../hand_images/hands";
    // nonhands_path = "../hand_images/other";
    if(hands_path=="")
      hands_path = "../hand_images/test/hands";
    if(nonhands_path=="")
      nonhands_path = "../hand_images/test/other";
    net->loadWeights(infile);
   }


   //nacitaj
   //net->loadWeights(infile);
   vector<string> hands = listDirectory(hands_path);
   vector<string> nonhands = listDirectory(nonhands_path);

   for(unsigned i = 0;i<hands.size();i++)
   {
     tests.push_back(make_pair(loadImage(hands[i],datatype,invert),make_vector(1)));
   }

   for(unsigned i = 0;i<nonhands.size();i++)
   {
     tests.push_back(make_pair(loadImage(nonhands[i],datatype,invert),make_vector(0)));
   }

   float E = 100;
   int epoche = 0;
   int good = 0;
   while(epoche<400 && (mode==0 || epoche<1))
   {
     epoche++;
     if(mode==0) cerr << epoche << endl;
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
       {
         if(abs(tests[i].second[0] - c)<0.5) good++;
         e = getError(c,tests[i].second[0]);
         cout << e << endl;
       }

       E += e;
     }
     cout << "Final error:" << E << endl;
     if(mode>0)
     {
       cout << "Good: " << good << " of " << tests.size() << " "
            << (100.0*good)/tests.size() << "%" << endl;
     }
     if(stop) break;
   }
   if(verbose)cout << epoche << endl;
   if(mode == 0) net->saveWeights(outfile);
   return 0;
}

int main(int argc, char *argv[])
{
    if(argc>0)
    {
        char ch = *(argv[1]);

        if(ch == 'r')
        {
            return recurrentTrain(argc,argv,1);
        }
        if(ch == 'n')
        {
            return normalTrain(argc,argv,1);
        }
    }

    cerr << "Invalid arguments" << endl;
    return 1;
}
