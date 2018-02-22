#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "basewindow.h"
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

signals:
    void updateBaseWindow();

private slots:
    void on_pushButton_clicked();

    void on_uidIn_returnPressed();

private:
    Ui::MainWindow *ui;
    BaseWindow * baseWindow;
    ContextManager localContext;

    void startNewSession();

    void endSession();
};

#endif // MAINWINDOW_H
