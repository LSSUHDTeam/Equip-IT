#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QFile>
#include <QScrollBar>
#include <QCloseEvent>
#include <QResizeEvent>
#include <QMainWindow>
#include <QByteArray>
#include <QStandardPaths>
#include <QCryptographicHash>
#include "framework/contextmanger.h"
#include "windows/reservationroot.h"
#include "windows/adminpanel.h"
#include "windows/checkout.h"

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

    void childWindowClosed(WindowDescriptors window_id);

signals:
    void isClosed();
    void forceCloseChildren();

private slots:
    void on_toolBox_currentChanged(int index);

    void on_authenticate_clicked();

    void on_r0c0_clicked();

    void on_r0c1_clicked();

    void on_r0c2_clicked();

    void on_r0c3_clicked();

    void on_r1c0_clicked();

    void on_r1c1_clicked();

    void on_r1c2_clicked();

    void on_r1c3_clicked();

    void on_r2c0_clicked();

    void on_r2c1_clicked();

    void on_r2c2_clicked();

    void on_r2c3_clicked();

    void on_r3c0_clicked();

    void on_r3c1_clicked();

    void on_r3c2_clicked();

    void on_r3c3_clicked();

    void on_clearButton_clicked();

    void on_checkoutButton_clicked();

    void on_addRemButton_clicked();

    void on_checkInButton_clicked();

    void on_overviewButton_clicked();

private:
    Ui::BaseWindow *ui;
    ContextManager * localContext;

    void shutdownWindow();
    void accessChildWindow(WindowDescriptors window);
    void addToBuff(QString item);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent * event);

    QString adminLoginBuffer;
};

#endif // BASEWINDOW_H
