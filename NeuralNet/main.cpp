#include <iostream>
#include "neuralnetwork.h"
#include "distributedneuralnetwork.h"
#include "distributedrecurrentnetwork.h"
#include "recurrentnetwork.h"
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <csignal>
#include <cstdio>
#include <sys/types.h>
#include <dirent.h>
#include <ctime>
#include <map>


using namespace std;
using namespace NeuralNET;

#define FOR(i,n) for(int i=0;i<(n);i++)
#define N_SIDE 128
#define N (N_SIDE * N_SIDE)
#define HIDDEN_N_SIDE 4
#define HIDDEN_N HIDDEN_N_SIDE*HIDDEN_N_SIDE*11
//#define HIDDEN_N 60
#define HIDDEN_N2 7
#define OUT_N 1
#define MAX_EPOCHE 400

double diffclock(clock_t clock1,clock_t clock2)
{
    double diffticks=clock1-clock2;
    double diffms=(diffticks*10000.0)/CLOCKS_PER_SEC;
    return diffms;
}

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
       if(s[0]=='.' || s=="nu") continue;
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

float alpha = 0.2;
int verbose = 0;
int mode = 0;
int datatype = 0;//0 - float data, 1 - image
bool invert = false;
string hands_path = "", nonhands_path = "";
string infile = "vahy", outfile;
vector<string> hands, others;

int recurrentTrain(unsigned sizes[], int maxepoche, int type = 1)
{
    vector<vector<vector<pair<vector<float>,vector<int > > > > > tests;//tests[seqIndex][frame][part]

    RecurrentNetwork *net = NULL;
    if(type==0)
        net = new RecurrentNetwork(2,sizes,N,alpha);
    else
        net = new DistributedRecurrentNetwork(2,sizes,HIDDEN_N_SIDE, HIDDEN_N_SIDE, N_SIDE,N_SIDE,alpha);

   //nacitaj   
   net->loadWeights(infile);   
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
   }
   for(unsigned i = 0;i<others.size();i++)
   {
       IndexInfo ii = parseFilename(others[i]);

       unsigned si = getIndex(ii.seqIndex);
       if(si>=tests.size())
       {
           tests.resize(si+1);
       }
       if(ii.frameIndex>=tests[si].size())
       {
           tests[si].resize(ii.frameIndex+1);
       }
       tests[si][ii.frameIndex].push_back(make_pair(loadImage(others[i],datatype,invert),make_vector(0)));
     //tests.push_back(make_pair(loadImage(nonhands[i],datatype,invert),make_vector(0)));
   }

   float E = 100;
   int epoche = 0;
   int good = 0, hgood = 0;
   clock_t begin=clock();
   while(epoche<maxepoche && (mode==0 || epoche<1))
   {
     good = 0, hgood = 0;
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

                 if(abs(tests[i][j][k].second[0] - c)<0.5)
                 {
                   good++;
                   if(tests[i][j][k].second[0]==1) hgood++;
                 }

                 if(mode==0)
                     e = net->train(tests[i][j][k].first,tests[i][j][k].second);
                 else
                 {
                   e = getError(c,tests[i][j][k].second[0]);
                   cout << e << endl;
                 }

                 //if(tests[i][j][k].second[0]==1)
                 if(c>0.5)
                 {
                     net->update();                     
                 }
                 E += e;
             }
         }
         net->reset();
     }
     cout << "Final error:" << E/(hands.size()+others.size())  << endl;
     cout << "Good: " << good << " of " << hands.size()+others.size() << " "
          << (100.0*good)/(hands.size()+others.size()) << "%" << endl;
     cout << "Good hands: " << hgood << " of " << hands.size() << " "
          << (100.0*hgood)/(hands.size()) << "%" << endl;
     cout << "Good other: " << good-hgood << " of " << others.size() << " "
          << (100.0*(good-hgood))/(others.size()) << "%" << endl;

     if(stop) break;
   }
   clock_t end=clock();
   if(verbose) cout << epoche << endl;
   cerr << "Time elapsed: " << double(diffclock(end,begin)) << " ms"<< endl;
   if(mode == 0) net->saveWeights(outfile);
   return 0;
}

int normalTrain(unsigned sizes[], int maxepoche, int type = 1)
{
   vector<pair<vector<float>,vector<int > > > tests;

   NeuralNetwork *net = NULL;
   if(type==0)
       net = new NeuralNetwork(2,sizes,N,alpha);
   else
       net = new DistributedNeuralNetwork(3,sizes,HIDDEN_N_SIDE, HIDDEN_N_SIDE, N_SIDE,N_SIDE,alpha);

   //nacitaj
   net->loadWeights(infile);
   for(unsigned i = 0;i<hands.size();i++)
   {
     tests.push_back(make_pair(loadImage(hands[i],datatype,invert),make_vector(1)));
   }
   for(unsigned i = 0;i<others.size();i++)
   {
     tests.push_back(make_pair(loadImage(others[i],datatype,invert),make_vector(0)));
   }

   float E = 100;
   int epoche = 0;
   int good = 0, hgood = 0;
   clock_t begin=clock();
   while(epoche<maxepoche && (mode==0 || epoche<1))
   {
     good = 0, hgood = 0;
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
         if(abs(tests[i].second[0] - c)<0.5)
         {
           good++;
           if(tests[i].second[0]==1)
             hgood++;
         }
         e = getError(c,tests[i].second[0]);
         cout << e << endl;
       }

       E += e;
     }
     cout << "Final error:" << E/tests.size() << endl;
     if(mode>0)
     {
       cout << "Good: " << good << " of " << tests.size() << " "
            << (100.0*good)/tests.size() << "%" << endl;
       cout << "Good hands: " << hgood << " of " << hands.size() << " "
            << (100.0*hgood)/(hands.size()) << "%" << endl;
       cout << "Good other: " << good-hgood << " of " << others.size() << " "
            << (100.0*(good-hgood))/(others.size()) << "%" << endl;
     }
     if(stop) break;
   }
   clock_t end=clock();
   if(verbose)cout << epoche << endl;
   cout << "Time elapsed: " << double(diffclock(end,begin)) << " ms"<< endl;
   if(mode == 0) net->saveWeights(outfile);
   return 0;
}

int main(int argc, char *argv[])
{
  int maxepoche = MAX_EPOCHE;
  srand(time(NULL)+42);
  signal(SIGINT, ctrlc);
  if(argc>0)
  {
    char ch = *(argv[1]);

    if(argc>2) maxepoche = atoi(argv[2]);
    if(maxepoche>0)
    {
      cerr << "Training " << maxepoche << " epoches." << endl;
      mode = 0;
    }
    else
    {
      cerr << "Testing." << endl;
      maxepoche = 1;
      mode = 1;
    }

    if(argc>3) verbose = atoi(argv[3]);
    if(argc>4) datatype = atoi(argv[4]);
    if(datatype==3)
    {
      datatype =1;
      invert = true;
    }

    if(argc>5) hands_path = argv[5];
    if(argc>6) nonhands_path = argv[6];
    if(argc>7) infile = argv[7];
    outfile = infile;
    if(argc>8) outfile = argv[8];

    if(mode==0)
    {
      if(hands_path=="")
        hands_path = "../hand_images/hands";
      if(nonhands_path=="")
        nonhands_path = "../hand_images/other";
    }
    else
    {
      // hands_path = "../hand_images/hands";
      // nonhands_path = "../hand_images/other";
      if(hands_path=="")
        hands_path = "../hand_images/test/hands";
      if(nonhands_path=="")
        nonhands_path = "../hand_images/test/other";
    }

    hands = listDirectory(hands_path);
    others = listDirectory(nonhands_path);

    int type = 1;
    //unsigned sizes[] = {HIDDEN_N, OUT_N};
    unsigned sizes[] = {HIDDEN_N, HIDDEN_N2, OUT_N};

    if(ch=='c')
    {
      cin >> ch;
      FOR(i,(int)(sizeof(sizes)/sizeof(unsigned))-1)
      {
        cin >> sizes[i];
        cerr << sizes[i] << endl;
      }
      if(type == 1)
        sizes[0]*=HIDDEN_N_SIDE*HIDDEN_N_SIDE;
    }
    if(ch == 'r')
    {
      return recurrentTrain(sizes, maxepoche,type);
    }
    if(ch == 'n')
    {
      return normalTrain(sizes, maxepoche,type);
    }
  }

  cerr << "Invalid arguments" << endl;
  return 1;
}
