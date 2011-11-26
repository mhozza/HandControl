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


