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

#include "color.h"

Color::Color(uchar r, uchar  g, uchar  b):QColor(r,g,b)
{
}

Color::Color(uint color):QColor(color)
{
  /*r = (color >> 16);
  g = ((color >> 8) & 0xff) ;
  b = (color & 0xff);*/
}

uint Color::toUintColor()
{
  return rgb();
  //return (0xFF000000 | b | (g << 8) | (r << 16));

}

uchar Color::toGrayScale()
{
  //return (4*r+3*g+3*b)/10;
  qGray(rgb());
}

string Color::toString()
{
  stringstream ss;
  ss << red()<< " " << green() << " " << blue() << " ";
  return ss.str();
}

