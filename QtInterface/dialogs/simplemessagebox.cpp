#include "simplemessagebox.h"
#include "ui_simplemessagebox.h"

SimpleMessageBox::SimpleMessageBox(smbdata messageData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimpleMessageBox)
{
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
    this->close();
}

void SimpleMessageBox::on_okayButton_clicked()
{
    this->close();
}

void SimpleMessageBox::closeEvent(QCloseEvent *event)
{
    emit messageBoxClosed();
    event->accept();
}
