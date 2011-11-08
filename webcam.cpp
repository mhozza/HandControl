/*
 *      webcam.cpp -- Kapture
 *
 *      Copyright (C) 2006-2009
 *          Detlev Casanova (detlev.casanova@gmail.com)
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>

#include <linux/videodev2.h>

#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QSize>
#include <QSocketNotifier>

#include "webcam.h"
#include "imageconvert.h"
#include "merror.h"


Webcam::Webcam()
{
	streaming = false;
	dev = 0;
	opened = false;
	imageNotifier = 0;
	allocated = false;
}

Webcam::~Webcam()
{
	close();
}

void Webcam::close()
{
	stopStreaming();
	::close(dev);

	delete imageNotifier;
	imageNotifier = 0;
	opened = false;
	allocated = false;
}

int Webcam::open()
{
	return this->open(m_name);
}

int Webcam::open(const QString& devFile)
{
	return this->open(devFile.toUtf8().constData());
}

int Webcam::open(const char *devFile)
{
	struct v4l2_capability cap;
	int ret;
	char str[256];

	m_name = QString(devFile);

	if (opened)
		close();

	dev = ::open(devFile, O_RDWR);
	if (dev < 0) 
	{
		strcpy(str, "Error Opening ");
		KError(strcat(str, devFile), errno);
		return EXIT_FAILURE;
	}

        memset(&cap, 0, sizeof cap);
        ret = ioctl(dev, VIDIOC_QUERYCAP, &cap);
        if (ret < 0) 
	{
		strcpy(str, "Error querying capabilities for ");
		KError(strcat(str, devFile), errno);
                return EXIT_FAILURE;
	}

	if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) 
	{
		strcpy(str, "Error checking caps for ");
		KError(strcat(str, devFile), errno);
                return -EINVAL;
	}
	opened = true;

	imageNotifier = new QSocketNotifier(dev, QSocketNotifier::Read);
	imageNotifier->setEnabled(false);
	connect(imageNotifier, SIGNAL(activated(int)), this, SIGNAL(imageReady()));
	return EXIT_SUCCESS;
}

QList<int> Webcam::getFormatList(QList<QString> &description) const
{
	QList<int> formatList;
	int ret;
	struct v4l2_fmtdesc fmtList;
	memset(&fmtList, 0, sizeof fmtList);
	fmtList.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int i = 0;

	do
	{
		fmtList.index = i;
		if ((ret = ioctl(dev, VIDIOC_ENUM_FMT, &fmtList)) < 0)
			break;
		else
		{
			formatList.append((int)fmtList.pixelformat);
			description.append((char*)fmtList.description);
		}
		i++;
	}
	while (ret != EINVAL);
	return formatList;
}

QList<QSize> Webcam::getSizesList() const
{
#ifdef VIDIOC_ENUM_FRAMESIZES
	int i = 0;
	QList<QSize> rSizes;
	QSize tmp;
	struct v4l2_frmsizeenum sizes;
	memset(&sizes, 0, sizeof sizes);
	sizes.pixel_format = currentPixelFormat();
	sizes.index = i;
	while(ioctl(dev, VIDIOC_ENUM_FRAMESIZES, &sizes) != -1)
	{
		tmp.setWidth((int)sizes.discrete.width);
		tmp.setHeight((int)sizes.discrete.height);
		rSizes.append(tmp);
		i++;
		sizes.index = i;
	}
	return rSizes;
#else
	QList<QSize> rSizes;
	QSize tmp;

	tmp.setWidth(320);
	tmp.setHeight(240);
	
	rSizes << tmp;

	return rSizes;
#endif
}


int Webcam::setFormat(unsigned int width, unsigned int height, int pixelformat)
{
	int i = 0;

	if(streaming)
		return -1;

	memset(&fmt, 0, sizeof fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = width;
	fmt.fmt.pix.height = height;
	fmt.fmt.pix.field = V4L2_FIELD_ANY;
	fmt.fmt.pix.pixelformat = pixelformat;
	if (ioctl(dev, VIDIOC_S_FMT, &fmt) < 0)
	{
		KError("Error while setting format", errno);
		i++;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int Webcam::streamOff()
{
	int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	int ret;
	
	if(!streaming)
		return -1;
	
	ret = ioctl(dev, VIDIOC_STREAMOFF, &type);
	if (ret < 0) 
	{
		KError("Unable to stop capture", errno);
		return EXIT_FAILURE;
	}
	streaming = false;
	return EXIT_SUCCESS;
}

int Webcam::startStreaming()
{
	int i, ret;

	if (!opened)
		return -1;

	if ((ret = setFormat(currentWidth(), currentHeight(), currentPixelFormat())) != 0)
	{
		printf("set format error : %d\n", ret);
		return EXIT_FAILURE;
	}

	//Allocate buffers 
	if (!allocated)
	{
		memset(&rb, 0, sizeof rb);
		rb.count = 2;
		rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		rb.memory = V4L2_MEMORY_MMAP;
	
		ret = ioctl(dev, VIDIOC_REQBUFS, &rb);
		if (ret < 0) 
		{
			KError("Unable to allocate buffers", errno);
			return EXIT_FAILURE;
		}
		allocated = true;
	}

			
	// Map the buffers. /
	memset(&buf, 0, sizeof buf);
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	for (i = 0; i < 2; i++)
	{
		buf.index = i; 
		ret = ioctl(dev, VIDIOC_QUERYBUF, &buf);
		if (ret < 0) {
			KError("Unable to query buffer", errno);
			return EXIT_FAILURE;
		}
		
		mem[i] = (uchar *) mmap(0, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, dev, buf.m.offset);
		if (mem[i] == MAP_FAILED) {
			KError("Unable to map buffer", errno);
			return EXIT_FAILURE;
		}
		bufLength = buf.length;
		mmaped = true;
	}

	// Queue the buffers. /
	for (i = 0; i < 2; i++)
	{
		buf.index = i;
		ret = ioctl(dev, VIDIOC_QBUF, &buf);
		if (ret < 0) 
		{
			KError("Unable to queue buffer", errno);
			return EXIT_FAILURE;
		}
	}

	// Start streaming.
	ret = ioctl(dev, VIDIOC_STREAMON, &buf.type);
	if (ret < 0) 
	{
		KError("Unable to start capture", errno);
		return EXIT_FAILURE;
	}
	
	imageNotifier->setEnabled(true);
	streaming = true;
	return EXIT_SUCCESS;
}

int Webcam::stopStreaming()
{
	if(!streaming)
		return -1;
	
	imageNotifier->setEnabled(false);
	
	if (munmap(mem[0], bufLength) == -1)
	{
		printf("Webcam::stopStreaming : munmap 0 failed. errno = %d\n", errno);
	}

	if (munmap(mem[1], bufLength) == -1)
	{
		printf("Webcam::stopStreaming : munmap 1 failed. errno = %d\n", errno);
	}
	else
		mmaped = false;

	if(streamOff() == 0)
	{
		streaming = false;
		printf(" * Succesful Stopped\n");
	}
	else
		imageNotifier->setEnabled(true);
	return EXIT_SUCCESS;
}

int Webcam::getFrame(QImage &image)
{
	int ret = 0;

	// Dequeue a buffer.
	ret = ioctl(dev, VIDIOC_DQBUF, &buf);
	if (ret < 0) 
	{
		KError("Unable to dequeue buffer", errno);
		return EXIT_FAILURE;
	}

	// Save the image.
	uchar jpegBuf1[buf.bytesused + 420];
	if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG)
	{
		if (mjpegToJpeg(mem[buf.index], jpegBuf1, (int) buf.bytesused) == EXIT_SUCCESS)
			image.loadFromData(jpegBuf1, buf.bytesused+420);
	}

	if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV)
	{
                yuvToJpeg(mem[buf.index], &image, currentWidth(), currentHeight());
	}
	
	// Requeue the buffer.
	ret = ioctl(dev, VIDIOC_QBUF, &buf);
	if (ret < 0) 
	{
		KError("Unable to requeue buffer", errno);
		return EXIT_FAILURE;
	}
	if(!imageNotifier->isEnabled())
		imageNotifier->setEnabled(true);
	
	return EXIT_SUCCESS;
}

int Webcam::getFrameBW(unsigned char *image)
{
        int ret = 0;

        // Dequeue a buffer.
        ret = ioctl(dev, VIDIOC_DQBUF, &buf);
        if (ret < 0)
        {
                KError("Unable to dequeue buffer", errno);
                return EXIT_FAILURE;
        }

        // Save the image.
        uchar jpegBuf1[buf.bytesused + 420];
        if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG)
        {
            /*
                if (mjpegToJpeg(mem[buf.index], jpegBuf1, (int) buf.bytesused) == EXIT_SUCCESS)
                        image.loadFromData(jpegBuf1, buf.bytesused+420);
            */
            return EXIT_FAILURE;
        }

        if (fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV)
        {
                yuvToBW(mem[buf.index], image, currentWidth(), currentHeight());
        }

        // Requeue the buffer.
        ret = ioctl(dev, VIDIOC_QBUF, &buf);
        if (ret < 0)
        {
                KError("Unable to requeue buffer", errno);
                return EXIT_FAILURE;
        }
        if(!imageNotifier->isEnabled())
                imageNotifier->setEnabled(true);

        return EXIT_SUCCESS;
}

int Webcam::changeCtrl(int ctrl, int value) // an enum for formats and reorganisation would be great...
{
	struct v4l2_queryctrl queryctrl;
	struct v4l2_control control;
	
	if(!opened) // At the begining of the function.
	{
		return -1;
	}
/*
 * ctrl values :
 * 	0 : Saturation
 * 	1 : Power line Frequency (néons)
 * 	2 : Brightness
 * 	3 : Contrast
 * 	4 : Sharpness
 * 	5 : Reset Pan/Tilt
 */
	__u32 CTRL;
	switch(ctrl)
	{
		case Saturation: 
		{
			CTRL = V4L2_CID_SATURATION;
			break;
		}
		case Brightness: 
		{
			CTRL = V4L2_CID_BRIGHTNESS;
			break;
		}
		case Contrast: 
		{
			CTRL = V4L2_CID_CONTRAST;
			break;
		}
#ifdef V4L2_CID_POWER_LINE_FREQUENCY
		case PowerLineFreq: 
		{
			CTRL = V4L2_CID_POWER_LINE_FREQUENCY;
			break;
		}
#endif
#ifdef V4L2_CID_SHARPNESS
		case Sharpness:
		{
			CTRL = V4L2_CID_SHARPNESS;
			break;
		}
#endif
#ifdef V4L2_CID_PANTILT_RESET
		case PanTiltReset:
		{
			CTRL = V4L2_CID_PANTILT_RESET;
			value = 3;
			break;
		}
#endif
		default:
			CTRL = 0;
	}

	memset (&queryctrl, 0, sizeof queryctrl);
	memset (&control, 0, sizeof control);
	queryctrl.id = CTRL;
	if (-1 == ioctl (dev, VIDIOC_QUERYCTRL, &queryctrl)) 
	{
	        if (errno != EINVAL) 
		{
#ifdef DEBUG
			perror ("VIDIOC_QUERYCTRL");
#endif
			return EXIT_FAILURE;
		} 
	} else 
	{
		control.id = CTRL;
		control.value = value;
		if (-1 == ioctl (dev, VIDIOC_S_CTRL, &control)) {
#ifdef DEBUG
			perror("VIDIOC_S_CTRL");
			printf(" * Error while setting control\n");
#endif
			return EXIT_FAILURE;
        	}
	}
	return EXIT_SUCCESS;
}

int Webcam::currentWidth() const
{
	return (int) fmt.fmt.pix.width;
}

int Webcam::currentHeight() const
{
	return (int) fmt.fmt.pix.height;
}

int Webcam::currentPixelFormat() const
{
	return (int) fmt.fmt.pix.pixelformat;
}


int Webcam::defaultCtrlVal(unsigned int control, int &defaultValue)
{
	struct v4l2_queryctrl queryctrl;
	QString ctrl;
	
	if(!opened)
	{
		return false;
	}
	
	memset(&queryctrl, 0, sizeof queryctrl);
	switch(control)
	{
		case Saturation : 
		{
			ctrl = "Saturation";
			queryctrl.id = V4L2_CID_SATURATION;
			break;
		}
		case Brightness : 
		{
			ctrl = "Brightness";
			queryctrl.id = V4L2_CID_BRIGHTNESS;
			break;
		}
		case Contrast : 
		{
			ctrl = "Contrast";
			queryctrl.id = V4L2_CID_CONTRAST;
			break;
		}
#ifdef V4L2_CID_POWER_LINE_FREQUENCY
		case PowerLineFreq : 
		{
			ctrl = "Powerline Frequecy";
			queryctrl.id = V4L2_CID_POWER_LINE_FREQUENCY;
			break;
		}
#endif
#ifdef V4L2_CID_SHARPNESS
		case Sharpness : 
		{
			ctrl = "Sharpness";
			queryctrl.id = V4L2_CID_SHARPNESS;
			break;
		}
#endif
		default :
			ctrl = "ERROR";
	}

	QString str;
	if (-1 == ioctl(dev, VIDIOC_QUERYCTRL, &queryctrl))
	{
		str = "Unable to set control ";
		printf("ERROR\n");
		KError(str + ctrl, errno);
		return false;
	}

	defaultValue = (int)queryctrl.default_value;

	return true;
}

bool Webcam::panTiltSupported()
{
#ifdef V4L2_CID_TILT_RELATIVE
	struct v4l2_queryctrl queryctrl;

	if(!opened)
	{
		return false;
	}

	memset(&queryctrl, 0, sizeof queryctrl);
	queryctrl.id = V4L2_CID_TILT_RELATIVE; // Could be V4L2_CID_PAN_RELATIVE.

	if (-1 == ioctl(dev, VIDIOC_QUERYCTRL, &queryctrl))
	{
		KError("Unable to check wether Pan Tilt is supported.", errno);
		return false;
	}

	if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
	{
		printf ("Pan & Tilt not supported.\n");
		return false; //FLAG_NOT_SUPPORTED;
	}

	return true;
#else
	return false;
#endif

}

void Webcam::turnRight()
{
#ifdef V4L2_CID_PAN_RELATIVE
	struct v4l2_queryctrl queryctrl;
	struct v4l2_control control;

	memset (&queryctrl, 0, sizeof queryctrl);
	memset (&control, 0, sizeof control);
	queryctrl.id = V4L2_CID_PAN_RELATIVE;
	if (-1 == ioctl (dev, VIDIOC_QUERYCTRL, &queryctrl)) 
	{
	        if (errno != EINVAL) 
		{
			perror ("VIDIOC_QUERYCTRL");
			return;
		} 
	} else 
	{
		control.id = V4L2_CID_PAN_RELATIVE;
		control.value = -320;
		if (-1 == ioctl (dev, VIDIOC_S_CTRL, &control)) {
			perror("VIDIOC_S_CTRL");
			return;
        	}
	}
#endif
}

void Webcam::turnLeft()
{
#ifdef V4L2_CID_PAN_RELATIVE
	struct v4l2_queryctrl queryctrl;
	struct v4l2_control control;

	memset (&queryctrl, 0, sizeof queryctrl);
	memset (&control, 0, sizeof control);
	queryctrl.id = V4L2_CID_PAN_RELATIVE;
	if (-1 == ioctl (dev, VIDIOC_QUERYCTRL, &queryctrl)) 
	{
	        if (errno != EINVAL) 
		{
			perror ("VIDIOC_QUERYCTRL");
			return;
		} 
	} else 
	{
		control.id = V4L2_CID_PAN_RELATIVE;
		control.value = 320;
		if (-1 == ioctl (dev, VIDIOC_S_CTRL, &control)) {
			perror("VIDIOC_S_CTRL");
			return;
        	}
	}
#endif
}

void Webcam::turnUp()
{
#ifdef V4L2_CID_TILT_RELATIVE
	struct v4l2_queryctrl queryctrl;
	struct v4l2_control control;
	
	memset (&queryctrl, 0, sizeof queryctrl);
	memset (&control, 0, sizeof control);
	queryctrl.id = V4L2_CID_TILT_RELATIVE;
	if (-1 == ioctl (dev, VIDIOC_QUERYCTRL, &queryctrl)) 
	{
	        if (errno != EINVAL) 
		{
			perror ("VIDIOC_QUERYCTRL");
			return;
		} 
	} else 
	{
		control.id = V4L2_CID_TILT_RELATIVE;
		control.value = -320;
		if (-1 == ioctl (dev, VIDIOC_S_CTRL, &control)) {
			perror("VIDIOC_S_CTRL");
			return;
        	}
	}
#endif
}

void Webcam::turnDown()
{
#ifdef V4L2_CID_TILT_RELATIVE
	struct v4l2_queryctrl queryctrl;
	struct v4l2_control control;

	memset (&queryctrl, 0, sizeof queryctrl);
	memset (&control, 0, sizeof control);
	queryctrl.id = V4L2_CID_TILT_RELATIVE;
	if (-1 == ioctl (dev, VIDIOC_QUERYCTRL, &queryctrl)) 
	{
	        if (errno != EINVAL) 
		{
			perror ("VIDIOC_QUERYCTRL");
			return;
		} 
	} else 
	{
		control.id = V4L2_CID_TILT_RELATIVE;
		control.value = 320;
		if (-1 == ioctl (dev, VIDIOC_S_CTRL, &control)) {
			perror("VIDIOC_S_CTRL");
			return;
        	}
	}
#endif
}

