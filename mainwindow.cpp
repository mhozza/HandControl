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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    camera = new Webcam();    
    setupCamera();

    connect(camera,SIGNAL(imageReady()),this,SLOT(getImage()));    
}

void MainWindow::setupCamera()
{
    if (!camera->isOpened())
    {
        camera->open(VIDEO_DEVICE);
        formatList = camera->getFormatList(formatName);
        camera->setFormat(VIDEO_WIDTH, VIDEO_HEIGHT, formatList.at(0));
        oldImage = new QImage(VIDEO_WIDTH, VIDEO_HEIGHT,QImage::Format_RGB32);

        for (int i = 0; i < formatName.size(); i++)
        {
               cout << formatList.at(i) << endl;
        }


        /*QList<QSize> sizes = camera->getSizesList();
        for (int i = 0; i < sizes.size(); i++)
        {
                sprintf(formatString, "%dx%d", sizes.at(i).width(), sizes.at(i).height() );
                if (sizes.at(i).width() == 320 && sizes.at(i).height() == 240)
                {
                        camera->setFormat(320, 240, formatList.at());
                }
        }*/
    }
}

void MainWindow::showEvent(QShowEvent * e)
{
    if(!camera->streaming())
    {
        setupCamera();

        ret = camera->startStreaming();
        if (ret == EXIT_FAILURE)
        {
                ui->label->setText("An error occured while restarting the camera.");
                //qDebug("***Unable to start streaming. Ret = %d\n", ret);
        }
    }
}

MainWindow::~MainWindow()
{
    disconnect(this,SLOT(getImage()));

    if(camera->isStreaming())
            camera->stopStreaming();
    camera->close();

    delete camera;

    delete ui;
}

QImage MainWindow::processImage(const QImage &image)
{
    QImage img(image);    

    unsigned sum = 0;
    for(int x = 0;x<img.width();x++)
    {
        for(int y = 0;y<img.height();y++)
        {
            QColor c1(image.pixel(x,y)),c2(oldImage->pixel(x,y));

            //greyscale difference
            uint g = abs((4*c1.red()+3*c1.green()+3*c1.blue()) - (4*c2.red()+3*c2.green()+3*c2.blue()))/10;

            //count pixels with difference> treshold
            if (g>TRESHOLD) sum++;

            QColor c(g,g,g);

            img.setPixel(x,y,c.rgb());

        }
    }

    //redscale image if big difference
    if(sum>img.width()*img.height()/RATIO)
    {
        for(int x = 0;x<img.width();x++)
        {
            for(int y = 0;y<img.height();y++)
            {
                img.setPixel(x,y,img.pixel(x,y)%256*256*256);
            }
        }
    }

    delete oldImage;
    oldImage = new QImage(image);

    return img;
}

void MainWindow::getImage()
{
        QPixmap pixmap;

        if (camera->getFrame(imageFromCamera) == EXIT_FAILURE)
        {                
                camera->close();
                throw new CameraGetImageFailureException();
                return;
        }

        if(!imageFromCamera.isNull())
        {
                //processImage(imageFromCamera);
                pixmap = QPixmap::fromImage(processImage(imageFromCamera));
        }

        if(!pixmap.isNull())
        {
                ui->label->setPixmap(pixmap);
        }
}

void MainWindow::startStopVideo()
{
        int ret = 0;
        if(camera->isStreaming())
        {
                if (camera->stopStreaming() == EXIT_SUCCESS)
                {
                        //ui->btnVideo->setText("Start video");
                }
        }
        else
        {
                if (!camera->isOpened())
                        camera->open(VIDEO_DEVICE);

                ret = camera->startStreaming();
                if (ret == EXIT_FAILURE)
                {
                        ui->label->setText("An error occured while restarting the camera.");
                        //qDebug("***Unable to start streaming. Ret = %d\n", ret);
                }
                else
                {
                        //ui->btnVideo->setText("Stop video");
                }
        }
}
