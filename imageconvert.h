/*
 *      imaeConvert.h -- Kapture
 *
 *      Copyright (C) 2006-2007
 *          Detlev Casanova (detlev.casanova@gmail.com)
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 */
#ifndef IMAGECONVERT_H
#define IMAGECONVERT_H

#include <vector>

using namespace std;

typedef unsigned char uchar;
typedef vector<uchar> ImageBuffer;

int mjpegToJpeg(unsigned char *inFrame, unsigned char *outFrame, unsigned int bufSize);
//int   yuvToJpeg(unsigned char *inFrame, unsigned char *outFrame, int width, int height);
int   yuvToJpeg(unsigned char *inFrame, QImage *outFrame, int width, int height);
//int   yuvToBW(unsigned char *inFrame, unsigned char *image, int width, int height);
ImageBuffer yuvToBW(unsigned char *inFrame, int width, int height);

#endif
