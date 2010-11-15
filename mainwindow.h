#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "webcam.h"

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
