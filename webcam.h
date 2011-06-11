/*
 *      webcam.h -- Kapture
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

#ifndef _WEBCAM_H
#define _WEBCAM_H

#include <QObject>
#include <QImage>
#include <QSocketNotifier>

#include <linux/videodev2.h>

class Webcam : public QObject
{
	Q_OBJECT

public:
	Webcam();
	~Webcam();
	
	void close();
	int  open();
	int  open(const char*);
	int  open(const QString&);
	QList<int>   getFormatList(QList<QString> &description) const;
	QList<QSize> getSizesList() const;
	int setFormat(unsigned int width, unsigned int height, int pixelformat=V4L2_PIX_FMT_MJPEG);
	int streamOff();
	int stopStreaming();
	int getFrame(QImage &image);
	int currentWidth() const;
	int currentHeight() const;
	int currentPixelFormat() const;
	int changeCtrl(int ctrl, int value = 0);
	int defaultCtrlVal(unsigned int control, int &defaultValue);
        bool isStreaming() const {return streaming;}
        bool isOpened() const {return opened;}
	bool panTiltSupported();
	QString name() const {return m_name;}

	enum Control {Saturation = 0,
		      PowerLineFreq,
		      Brightness,
		      Contrast,
		      Sharpness,
		      PanTiltReset};
	
signals:
	void imageReady();

public slots:
	int startStreaming();
	void turnRight();
	void turnLeft();
	void turnUp();
	void turnDown();

private:
	int dev;
	v4l2_format fmt;
	v4l2_buffer buf;
	v4l2_requestbuffers rb;
	bool allocated;
	
	uchar *mem[2];
	size_t bufLength;
	QSocketNotifier *imageNotifier;
	QImage *image;
	bool streaming;
	bool opened;
	bool mmaped;
	QString m_name;
};
#endif

