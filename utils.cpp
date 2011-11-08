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
        ofs << grayScale(img.pixel(x,y)) << " ";
      }
      ofs << endl;
    }
    ofs.close();
}

