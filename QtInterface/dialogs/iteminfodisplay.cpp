#include "iteminfodisplay.h"
#include "ui_iteminfodisplay.h"

ItemInfoDisplay::ItemInfoDisplay(ItemInfoWrap item, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemInfoDisplay)
{
    ui->setupUi(this);
    itemInfo = item;
    buildDisplay();
}

ItemInfoDisplay::~ItemInfoDisplay()
{
    delete ui;
}

void ItemInfoDisplay::buildDisplay()
{
    ui->label->setText(itemInfo.item.name);
    ui->textBrowser->insertPlainText("\n\nBarcode:\t\t");
    ui->textBrowser->insertPlainText(itemInfo.item.barcode);
    ui->textBrowser->insertPlainText("\n\nDescription:\t\t");
    ui->textBrowser->insertPlainText(itemInfo.item.desc);
    ui->textBrowser->insertPlainText("\n\nPeripherals:\n");
    for(auto i = itemInfo.item.periphs.begin();
            i != itemInfo.item.periphs.end(); ++i)
    {
        ui->textBrowser->insertPlainText("\t\t");
        ui->textBrowser->insertPlainText((*i).name);
        ui->textBrowser->insertPlainText(":\n\t\t\t");
        ui->textBrowser->insertPlainText((*i).desc);
        ui->textBrowser->insertPlainText("\n\n\t\tNumber Expected: ");
        ui->textBrowser->insertPlainText(QString::number((*i).count));
        ui->textBrowser->insertPlainText("\n\t\tNumber Present: ");
        ui->textBrowser->insertPlainText(QString::number((*i).numberpresent));
        ui->textBrowser->insertPlainText("\n\n");
    }
}

void ItemInfoDisplay::on_closeButton_clicked()
{
    this->close();
}
