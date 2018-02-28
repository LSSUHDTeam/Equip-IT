#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QStandardPaths>
#include <QMainWindow>
#include "basewindow.h"
#include "displaynetworkerror.h"
#include "framework/windowdescriptors.h"
#include "framework/contextmanger.h"

#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void hideBaseWindow();

    // From DAM via context manager
    void dataReady();
    void networkErrorPresent(DAMStatus);

    // From network error display
    void networkErrorDialogRetrySignaled();
    void networkErrorIgnore();

signals:
    void updateBaseWindow();

private slots:
    void on_pushButton_clicked();

    void on_uidIn_returnPressed();

private:
    Ui::MainWindow *ui;
    BaseWindow * baseWindow;
    ContextManager localContext;

    bool waitingForUpdate, idle;

    void startNewSession();

    void endSession();
};

#endif // MAINWINDOW_H
