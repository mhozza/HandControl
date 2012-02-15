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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

#include "webcam.h"
#include "exceptions.h"
#include "imageprocessor.h"
#include "handrecognizer.h"
#include "gesturerecognizer.h"
#include "onscreendisplayform.h"
#include "hcimage.h"

#define VIDEO_DEVICE "/dev/video0"
#define VIDEO_WIDTH 400
#define VIDEO_HEIGHT 300
#define NO_HAND_FRAMES 7

namespace Ui {
    class MainWindow;
}

using namespace handcontrol;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Webcam *camera;
    HCImage imageFromCamera;

    QList<QString> formatName;
    QList<int> formatList;
    ImageProcessor *imageProcessor;
    HandRecognizer * handRecognizer;
    GestureRecognizer * gestureRecognizer;
    OnScreenDisplayForm osd;

    int noHandFrames;

    void setupCamera();
protected:
    void showEvent(QShowEvent *);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void getImage();        
    void toggleKalmann(bool state);
};

#endif // MAINWINDOW_H
