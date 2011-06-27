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

#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    noHandFrames(0)
{
    ui->setupUi(this);
    camera = new Webcam();        
    handRecognizer = new HandRecognizer();
    gestureRecognizer= new GestureRecognizer();
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

        imageProcessor = new ImageProcessor(VIDEO_WIDTH, VIDEO_HEIGHT, handRecognizer);

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
    delete handRecognizer;
    delete gestureRecognizer;

    delete ui;
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
                QImage img =
                imageProcessor->processImage(imageFromCamera);
                //QImage img(VIDEO_WIDTH,VIDEO_HEIGHT,QImage::Format_RGB32);
                QPainter p;
                p.begin(&img);
                /*p.setPen(QPen(QColor(Qt::white)));
                p.setBrush(QBrush(QColor(Qt::white)));
                p.drawRect(0,0,VIDEO_WIDTH,VIDEO_HEIGHT);*/
                if(handRecognizer->isHand())
                {
                  noHandFrames = 0;
                  p.setPen(QPen(QColor(Qt::blue)));
                  p.setBrush(QBrush(QColor(Qt::red)));
                  QRect r = handRecognizer->getHandRect();
                  p.drawRect(r.x()+r.width()/2-2,r.y()+r.height()/2-2,4,4);

                  p.setPen(QPen(QColor(Qt::red)));
                  p.setBrush(QBrush(QColor(Qt::color0), Qt::NoBrush));
                  p.drawRect(r);

                  ui->label_2->setText(QString::number(handRecognizer->getHandP()));

                  QPoint point(r.x()+r.width()/2,r.y()+r.height()/2);
                  gestureRecognizer->addPoint(point);
                  Gesture *g = gestureRecognizer->getGesture();
                  if(g!=NULL)
                  {
                    g->action();
                    osd.setText(g->text());
                    gestureRecognizer->resetGesture();
                    osd.show();
                  }
                }
                else
                {
                  noHandFrames++;
                  if(noHandFrames>=NO_HAND_FRAMES)
                  {                    
                    gestureRecognizer->resetGesture();
                  }
                }
                gestureRecognizer->drawPoints(&p);
                p.end();          

                pixmap = QPixmap::fromImage(img);
        }

        if(!pixmap.isNull())
        {
                ui->label->setPixmap(pixmap);
                ui->radioButton->setChecked(imageProcessor->imageChanged());
        }
}
