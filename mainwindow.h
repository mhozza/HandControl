#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "webcam.h"

#define VIDEO_DEVICE "/dev/video0"
#define TRESHOLD 16
#define RATIO 24

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Webcam *camera;
    QImage imageFromCamera;
    const QImage *oldImage;
    QList<QString> formatName;
    QList<int> formatList;

    QImage processImage(const QImage &img);
protected:
    void showEvent(QShowEvent *);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void getImage();
    void startStopVideo();
};

#endif // MAINWINDOW_H
