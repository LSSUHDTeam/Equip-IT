#include "adminpanel.h"
#include "ui_adminpanel.h"

AdminPanel::AdminPanel(ContextManager * context, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminPanel)
{
    ui->setupUi(this);
    localContext = context;
    this->showMaximized();
}

AdminPanel::~AdminPanel()
{
    localContext = 0;
    delete ui;
}

void AdminPanel::requestForceClose()
{
    this->close();
}

void AdminPanel::on_setPasswordButton_clicked()
{
    if(ui->buffEdit->text().length()==0)
    {
        ui->statusbar->showMessage("Nothing to set.");
        ui->infoLabel->setText("Fail");
        return;
    }
    QString hashFile = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/resources/ahash.eqpt";
    QByteArray h = QCryptographicHash::hash(ui->buffEdit->text().toLocal8Bit(), QCryptographicHash::Sha256);
    ui->buffEdit->clear();
    QFile tfile(hashFile);
    if(tfile.open(QIODevice::ReadWrite))
    {
        tfile.write(h);
    }
    tfile.close();
    localContext->addUserCrumb("Admin panel password changed.");
    ui->infoLabel->setText("Success");
}

void AdminPanel::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    emit adminPanelClosed(WindowDescriptors::AdminPanel);
}

void AdminPanel::on_b1_clicked()
{
    ui->buffEdit->insert("1");
}

void AdminPanel::on_b2_clicked()
{
    ui->buffEdit->insert("2");
}

void AdminPanel::on_b3_clicked()
{
    ui->buffEdit->insert("3");
}

void AdminPanel::on_b4_clicked()
{
    ui->buffEdit->insert("4");
}

void AdminPanel::on_b5_clicked()
{
    ui->buffEdit->insert("5");
}

void AdminPanel::on_b6_clicked()
{
    ui->buffEdit->insert("6");
}

void AdminPanel::on_b7_clicked()
{
    ui->buffEdit->insert("7");
}

void AdminPanel::on_b8_clicked()
{
    ui->buffEdit->insert("8");
}

void AdminPanel::on_b9_clicked()
{
    ui->buffEdit->insert("9");
}

void AdminPanel::on_b0_clicked()
{
    ui->buffEdit->insert("0");
}

void AdminPanel::on_bA_clicked()
{
    ui->buffEdit->insert("A");
}

void AdminPanel::on_bB_clicked()
{
    ui->buffEdit->insert("B");
}

void AdminPanel::on_bC_clicked()
{
    ui->buffEdit->insert("C");
}

void AdminPanel::on_bD_clicked()
{
    ui->buffEdit->insert("D");
}

void AdminPanel::on_bE_clicked()
{
    ui->buffEdit->insert("E");
}

void AdminPanel::on_bE_2_clicked()
{
    ui->buffEdit->insert("F");
}

void AdminPanel::on_clearButton_clicked()
{
    ui->buffEdit->clear();
    ui->infoLabel->setText("Cleared");
}

void AdminPanel::on_calendarWidget_clicked(const QDate &date)
{
    qDebug() << "You clicked (day.month.year): " << date.toString("dd.MM.yyyy");
    localContext->addUserCrumb("Log viewer date (dmy)> " + date.toString("dd.MM.yyyy"));
}

void AdminPanel::on_tableWidget_cellClicked(int row, int column)
{
    qDebug() << "Cell: " << row << ", " << column << " was clicked once.";
}

void AdminPanel::on_tableWidget_cellDoubleClicked(int row, int column)
{
    qDebug() << "Cell: " << row << ", " << column << " was double clicked.";
}

void AdminPanel::on_tabWidget_currentChanged(int index)
{
    switch(index){
    case 0:
        localContext->addUserCrumb("On tab 'password changer.'");
        break;
    case 1:
        localContext->addUserCrumb("On tab 'log viewer.'");
        break;
    }
}
