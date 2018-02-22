#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QCloseEvent>
#include <QResizeEvent>
#include <QMainWindow>
#include "framework/contextmanger.h"

namespace Ui {
class BaseWindow;
}

class BaseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BaseWindow(ContextManager *context, QWidget *parent = 0);
    ~BaseWindow();

public slots:
    void performUpdateOnWindow();

    void sessionTimeout();

    void updateSessionTimeRemainingOutput(QString timeRemaining);

signals:
    void isClosed();

private:
    Ui::BaseWindow *ui;
    ContextManager * localContext;

    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent * event);
};

#endif // BASEWINDOW_H
