#include "simplemessagebox.h"
#include "ui_simplemessagebox.h"

SimpleMessageBox::SimpleMessageBox(smbdata messageData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimpleMessageBox)
{
    forced = false;
    ui->setupUi(this);
    ui->fromLabel->setText(messageData.from);
    ui->titleLabel->setText(messageData.title);
    foreach(QString line, messageData.messageData)
    {
        ui->messageText->insertPlainText(line + "\n");
    }
}

SimpleMessageBox::~SimpleMessageBox()
{
    delete ui;
}

void SimpleMessageBox::forceClose()
{
    forced = true;
    this->close();
}

void SimpleMessageBox::on_okayButton_clicked()
{
    this->close();
}

void SimpleMessageBox::closeEvent(QCloseEvent *event)
{
    if(!forced)
    {
        this->hide();
        emit messageBoxClosed();
    }
    event->accept();
}
