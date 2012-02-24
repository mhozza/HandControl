/*
 *    HandControl - Hand gesture recognition
 *    Copyright (C) 2010  Michal Hozza (mhozza@gmail.com)
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "utils.h"

void Utils::saveImage(QImage img, int index = 0)
{
    //zapis do suboru
    stringstream fname;
    fname << "hand_images/"<<  "full" << "_" << index << ".pbm";

    ofstream ofs(fname.str().c_str());
    ofs << "P2" << endl;
    ofs << img.width() << " " << img.height() << endl;
    ofs << 256 << endl;
    for(int y = 0;y < img.height(); y++)
    {
      for(int x = 0;x < img.width(); x++)
      {
        QColor c = img.pixel(x,y);         ;
        ofs << ((3*c.red()+3*c.green()+4*c.blue())/10) << " ";
      }
      ofs << endl;
    }
    ofs.close();
}

void Utils::saveImage(HCImage img, int index = 0, string fname = "")
{    
    if(fname==""){
        //zapis do suboru
        stringstream ssfname;
        ssfname << "hand_images/"<<  "full" << "_" << index << ".pbm";
        fname = ssfname.str();
    }

    ofstream ofs(fname.c_str());

    ofs << "P2" << endl;
    ofs << img.width() << " " << img.height() << endl;
    ofs << 256 << endl;
    for(int y = 0;y < img.height(); y++)
    {
      for(int x = 0;x < img.width(); x++)
      {
        ofs << (int)img.pixel(x,y) << " ";
      }
      ofs << endl;
    }
    ofs.close();
}

double Utils::cabs(fftw_complex v)
{
    return sqrt(v[0]*v[0]+v[1]*v[1]);
}
/*
//Matrix
template <class T> vector<vector<T> > Matrix<T>::createIdentity(int w, int h)
{
  vector<vector<T> m;
  m.resize(h);
  for(unsigned i = 0;i<m.size();i++)
  {
    m[i].resize(w,T(0));
    if(i<w)
      m[i][i] = T(1);
  }
}

template <class T> vector<vector<T> > Matrix<T>::transpone(vector<vector<T> > v)
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

//Matrix operators

template <class T> vector<T> operator+(vector<T> a, vector<T> b)
{
  //if(a.size()!=b.size) throw -1;
  vector<T> c;
  c.resize(0);
  for(unsigned i = 0;i<a.size();i++)
  {
    c.push_back(a[i]+b[i]);
  }
  return c;
}

template <class T> vector<vector<T> > operator+(vector<vector<T> > a, vector<vector<T> > b)
{
  //if(a.size()!=b.size) throw -1;
  vector<vector<T> > c;
  c.resize(0);
  for(unsigned i = 0;i<a.size();i++)
  {
    c.push_back(a[i]+b[i]);
  }
  return c;
}

template <class T> T operator*(vector<T> a, vector<T> b) //!!skalarny sucin
{
  //if(a.size()!=b.size) throw -1;
  T c = T(0);
  for(unsigned i = 0;i<a.size();i++)
  {
    c+=a[i]*b[i];
  }
  return c;
}

template <class T> vector<vector<T> > operator+(vector<vector<T> > a, vector<vector<T> > b) //!!maticove nasobenie
{
  vector<vector<T> > bb = Matrix<T>::transpone(b);

}
*/
