#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup an appdata folder
    QString addin_path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(addin_path);
    if (!dir.exists())
        dir.mkpath(addin_path);
    if (!dir.exists("resources"))
        dir.mkdir("resources");

    // Ensure a buildings.json file exists
    QString buildingsFile = addin_path + "/resources/buildings.json";
    QFileInfo check(buildingsFile);
    if (!check.exists())
    {
        QFile bldgfile(buildingsFile);
        if (bldgfile.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&bldgfile);
            stream << "[]" << endl;
        }
        bldgfile.close();
    }

    this->showMaximized();
    idle = true;
    waitingForUpdate = true;
    ui->uidIn->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->statusBar->showMessage("Waiting for data sync....");
    ui->uidIn->setFocus();

    // The base window never really goes away, it just stays hidden until a user is authd
    baseWindow = new BaseWindow(&localContext, this);
    connect(baseWindow, SIGNAL(isClosed()), this,  SLOT(hideBaseWindow()));
    connect(this, SIGNAL(updateBaseWindow()), baseWindow,  SLOT(performUpdateOnWindow()));

    // DAM connetion
    connect(&localContext, SIGNAL(rippleDataReady()), this, SLOT(dataReady()));
    connect(&localContext, SIGNAL(rippleNetworkError(DAMStatus)), this, SLOT(networkErrorPresent(DAMStatus)));
}

MainWindow::~MainWindow()
{
    delete baseWindow;
    delete ui;
}

void MainWindow::dataReady()
{
    if (waitingForUpdate)
    {
        qDebug() << "Waiting for update, and data is ready";
        waitingForUpdate = false;
        ui->uidIn->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->statusBar->showMessage("Ready");
        ui->uidIn->setFocus();
    }
}

void MainWindow::hideBaseWindow()
{
    idle = true;
    baseWindow->hide();
    ui->uidIn->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->statusBar->showMessage("Ready");
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
    idle = false;
    ui->uidIn->clear();
    ui->uidIn->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->statusBar->showMessage("Waiting for user session to end.");
    localContext.changeUserLocation(WindowDescriptors::BaseWindow);
    emit updateBaseWindow();
    baseWindow->showMaximized();
}

void MainWindow::endSession()
{
    ui->statusBar->clearMessage();
    ui->uidIn->setFocus();
}

void MainWindow::networkErrorPresent(DAMStatus status)
{
    if(idle)
    {
        ui->statusBar->showMessage("...Network Error...");
        DisplayNetworkError *dne = new DisplayNetworkError(
                    &localContext ,
                    NetworkCallerConfig(NetworkCallerOrigin::primary, status, true),
                    this);
        connect(dne, SIGNAL(retrying()), this, SLOT(networkErrorDialogRetrySignaled()));
        connect(dne, SIGNAL(ignoreIssue()), this, SLOT(networkErrorIgnore()));
        dne->setAttribute(Qt::WA_DeleteOnClose, true);
        dne->show();
    }
}

void MainWindow::networkErrorDialogRetrySignaled()
{
    ui->statusBar->showMessage("Retrying data sync...");
}

void MainWindow::networkErrorIgnore()
{
    ui->statusBar->showMessage("Network errors ignored. EquipIT will keep trying to connect.");
}

void MainWindow::on_pushButton_clicked()
{
    startNewSession();
}

void MainWindow::on_uidIn_returnPressed()
{
    startNewSession();
}
