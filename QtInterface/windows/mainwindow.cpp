#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // The base window never really goes away, it just stays hidden until a user is authd
    baseWindow = new BaseWindow(&localContext, this);
    connect(baseWindow, SIGNAL(isClosed()), this,  SLOT(hideBaseWindow()));
    connect(this, SIGNAL(updateBaseWindow()), baseWindow,  SLOT(performUpdateOnWindow()));
}

MainWindow::~MainWindow()
{
    delete baseWindow;
    delete ui;
}

void MainWindow::hideBaseWindow()
{
    baseWindow->hide();
    this->show();
}

void MainWindow::startNewSession()
{
    if (!localContext.createSession(ui->uidIn->text()))
    {
        ui->statusBar->showMessage("Invalid User ID");
        ui->uidIn->clear();
        ui->uidIn->setFocus();
        return;
    }
    ui->uidIn->clear();
    this->hide();
    emit updateBaseWindow();
    baseWindow->show();
}

void MainWindow::endSession()
{
    ui->statusBar->clearMessage();
    ui->uidIn->setFocus();
}

void MainWindow::on_pushButton_clicked()
{
    startNewSession();
}

void MainWindow::on_uidIn_returnPressed()
{
    startNewSession();
}
