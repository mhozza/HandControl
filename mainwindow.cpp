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

    camera->open("/dev/video0");
    formatList = camera->getFormatList(formatName);
    camera->setFormat(640, 480, formatList.at(0));
    oldImage = new QImage(640,480,QImage::Format_RGB32);

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

    connect(camera,SIGNAL(imageReady()),this,SLOT(getImage()));    
}

void MainWindow::showEvent(QShowEvent * e)
{
    startStopVideo();
}

MainWindow::~MainWindow()
{
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
                //startStopVideo();
                camera->close();
                //fctExecuted = 0;
                //ui.comboBoxSize->disconnect(SIGNAL( currentIndexChanged(const QString &) ), this, SLOT(changeFormat(const QString &) ));
                //waitCamera.start(2000);
                return;
        }

        if(!imageFromCamera.isNull())
        {
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
