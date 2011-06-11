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
        if(camera->open(VIDEO_DEVICE)==-1) throw new CameraOpenException();

        formatList = camera->getFormatList(formatName);
        camera->setFormat(VIDEO_WIDTH, VIDEO_HEIGHT, formatList.at(0));
        imageProcessor = new ImageProcessor(VIDEO_WIDTH, VIDEO_HEIGHT);

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
    try
    {
        if(!camera->isStreaming())
        {
            setupCamera();

            int ret = camera->startStreaming();
            if (ret == EXIT_FAILURE)
            {
                    ui->label->setText("An error occured while restarting the camera.");
                    throw new CameraStartStreamingFailureException();
                    //qDebug("***Unable to start streaming. Ret = %d\n", ret);
            }
        }
    }
    catch(Exception &e)
    {
        cout << e.what() << endl;
    }
}

MainWindow::~MainWindow()
{
    disconnect(this,SLOT(getImage()));

    if(camera->isStreaming())
            camera->stopStreaming();
    camera->close();

    delete camera;
    delete imageProcessor;

    delete ui;
}


void MainWindow::getImage()
{
        QPixmap pixmap, pixmap3;

        if (camera->getFrame(imageFromCamera) == EXIT_FAILURE)
        {                
                camera->close();
                throw new CameraGetImageFailureException();
                return;
        }

        if(!imageFromCamera.isNull())
        {
                //processImage(imageFromCamera);
                QImage img = imageProcessor->processImage(imageFromCamera);
                HandDetector d;
                img.setPixel(d.getHand(&img),0xFFFF0000);
                pixmap = QPixmap::fromImage(img);                
                //pixmap3 = QPixmap::fromImage(imageFromCamera);
        }

        if(!pixmap.isNull())
        {
                ui->label->setPixmap(pixmap);                
                //ui->label_3->setPixmap(pixmap3);
                ui->radioButton->setChecked(imageProcessor->imageChanged());
        }
}



