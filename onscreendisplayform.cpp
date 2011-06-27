#include "onscreendisplayform.h"
#include "ui_onscreendisplayform.h"

OnScreenDisplayForm::OnScreenDisplayForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnScreenDisplayForm)
{
    ui->setupUi(this);    
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    hideTimer.setInterval(1000);
    hideTimer.setSingleShot(true);
    connect(&hideTimer,SIGNAL(timeout()),this,SLOT(hide()));
}

void OnScreenDisplayForm::setText(QString text)
{
  ui->label->setText(text);
}

OnScreenDisplayForm::~OnScreenDisplayForm()
{
    delete ui;
}

void OnScreenDisplayForm::showEvent(QShowEvent *)
{
  QRect r = this->frameGeometry();
  r.moveCenter(QApplication::desktop()->availableGeometry().center());
  this->move(r.topLeft());
  hideTimer.start();
}
