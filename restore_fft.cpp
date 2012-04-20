/*
 *    Copyright (C) 2012  Michal Hozza (mhozza@gmail.com)
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
#include "grayscaleimage.h"
#include <fstream>
#include <fftw3.h>
#include "utils.h"

using namespace std;

int main(int argc, char * argv[])
{
    if(argc>1)
    {
        string fname = argv[1];
        GrayScaleImage img;
        img.loadFromPPM(fname);

        //fft
        fftw_complex *in = NULL;
        fftw_complex *out = NULL;
        fftw_plan p;
        in = img.toComplexArray();
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * img.width() * img.height());
        p = fftw_plan_dft_2d(img.width(), img.height(), in, out, FFTW_FORWARD ,FFTW_ESTIMATE | FFTW_DESTROY_INPUT);
        fftw_execute(p); // repeat as needed
        fftw_destroy_plan(p);

        img.setImageFromComplexArray(out,img.width(),img.height());

        img.saveImage(0,fname.substr(0,fname.size()-3)+"trn.pbm");

        ofstream ofs((fname.substr(0,fname.size()-3)+"trn").c_str());
        for(unsigned y = 0;y < img.height(); y++)
        {
          for(unsigned x = 0;x < img.width(); x++)
          {
            ofs << 1/(1+Utils::cabs(out[x+y*img.width()])) << " ";
          }
          ofs << endl;
        }

    }
    return 0;
}
