#ifndef ONSCREENDISPLAYFORM_H
#define ONSCREENDISPLAYFORM_H

#include <QWidget>
#include <QTimer>
#include <QDesktopWidget>

namespace Ui {
    class OnScreenDisplayForm;
}

class OnScreenDisplayForm : public QWidget
{
    Q_OBJECT
    QTimer hideTimer;
protected:
    void showEvent(QShowEvent *);
public:
    explicit OnScreenDisplayForm(QWidget *parent = 0);
    void setText(QString text);
    ~OnScreenDisplayForm();

private:
    Ui::OnScreenDisplayForm *ui;
};

#endif // ONSCREENDISPLAYFORM_H
