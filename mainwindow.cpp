#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    connect(ui->btnVideo,SIGNAL(clicked()),this,SLOT(startStopVideo()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QImage MainWindow::processImage(const QImage &image)
{
    QImage img(image);

    /*
     detekcia velkych zmien -
     zratat pixely, ktore maju vacsi rozdiel ako je nejaka konstanta
     a ak ich je viac ako nejaka konstanta pomeru k velkosti obrazu, zahlas zmenu
     */

    for(int x = 0;x<img.width();x++)
    {
        for(int y = 0;y<img.height();y++)
        {
            //QColor c1(image.pixel(x,y)),c2(oldImage->pixel(x,y));
            uint color = abs(image.pixel(x,y)-oldImage->pixel(x,y));
            //if (color<2000000) color = 0;

            img.setPixel(x,y,color);
            //(QString(image.pixel(x,y)-oldImage->pixel(x,y)).constData());
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
                //fctExecuted = 0;StartStop
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
               /* if(crIsActivated)
                {
                        //Show the modified Picture (Color Replaced).
                        showColorReplaced();
                }*/
                //showZoom();

        }
}

void MainWindow::startStopVideo()
{
        int ret = 0;
        if(camera->isStreaming())
        {
                if (camera->stopStreaming() == EXIT_SUCCESS)
                {
                        ui->btnVideo->setText("Start video");
                        //keepZoomer.start(100);
                }
        }
        else
        {
                if (!camera->isOpened())
                        camera->open("/dev/video0");

                ret = camera->startStreaming();
                if (ret == EXIT_FAILURE)
                {
                        ui->label->setText("An error occured while restarting the camera.");
                        //qDebug("***Unable to start streaming. Ret = %d\n", ret);
                }
                else
                {
                        ui->btnVideo->setText("Stop video");
                        //keepZoomer.stop();
                }
        }
}
