#include "quickscanitem.h"
#include "ui_quickscanitem.h"

QuickScanItem::QuickScanItem(ContextManager *context, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuickScanItem)
{
    ui->setupUi(this);
    this->showMaximized();
    noPeriphs = false;
    currentState = QSIState::ScanWindow;
    localContext = context;
    currentItems = localContext->getExistingItems();
    ui->toolBox->setCurrentIndex(0);
    ui->lineEdit->setFocus();
    peripheralList << "Name" << "Description" << "Count" << "Number Present";


    ui->periphralTable->setColumnCount(peripheralList.length());
    ui->periphralTable->setHorizontalHeaderLabels(peripheralList);
    ui->periphralTable->setColumnWidth(0, ui->periphralTable->width()/4);
    ui->periphralTable->setColumnWidth(1, ui->periphralTable->width()/4);
    ui->periphralTable->setColumnWidth(2, ui->periphralTable->width()/4);
    ui->periphralTable->setColumnWidth(3, ui->periphralTable->width()/4-25);
}

QuickScanItem::~QuickScanItem()
{
    localContext = 0;
    delete ui;
}

void QuickScanItem::forceClose()
{
    this->close();
}

void QuickScanItem::on_lineEdit_returnPressed()
{
    buildConfirmationWindow();
}

void QuickScanItem::on_pushButton_clicked()
{
    buildConfirmationWindow();
}

void QuickScanItem::buildConfirmationWindow()
{
    QString barcode_search = ui->lineEdit->text();
    if (barcode_search.length() == 0)
    {
        ui->lineEdit->setFocus();
        ui->errorLabel->setText("Error: Nothing in search field");
        return;
    }
    localContext->addUserCrumb("Searching for barcode: "+barcode_search);
    bool found = false;
    reservableItems itemScanned;
    for(auto i = currentItems.begin(); ((!found)&&(i != currentItems.end())); ++i)
    {
        if((*i).barcode == barcode_search)
        {
            itemScanned = (*i);
            found = true;
        }
    }
    if (!found)
    {
        ui->errorLabel->setText("Error: Item not found");
        return;
    }

    qDebug() << "Item scanned: " << itemScanned.name;
    itemFound = itemScanned;

    ui->nameLabel->setText(itemFound.name);
    ui->barcodeLabel->setText(itemFound.barcode);

    /*



                BUILD THE PERIPHERAL TABLE HERE


    */
    ui->periphralTable->setRowCount(itemFound.periphs.size());

    if(itemFound.periphs.size() == 0)
        noPeriphs = true;
    else
        noPeriphs = false;

    int loc = 0;
    for(auto i = itemFound.periphs.begin(); i!= itemFound.periphs.end(); ++i)
    {
        QTableWidgetItem *name = new QTableWidgetItem();
        name->setText((*i).name);
        name->setFlags(name->flags() ^ Qt::ItemIsEditable);
        ui->periphralTable->setItem(loc, 0, name);

        QTableWidgetItem *desc = new QTableWidgetItem();
        desc->setText((*i).desc);
        desc->setFlags(desc->flags() ^ Qt::ItemIsEditable);
        ui->periphralTable->setItem(loc, 1, desc);

        QTableWidgetItem *count = new QTableWidgetItem();
        count->setText( QString::number( (*i).count ));
        count->setFlags(count->flags() ^ Qt::ItemIsEditable);
        ui->periphralTable->setItem(loc, 2, count);

        QTableWidgetItem *numberpresent = new QTableWidgetItem();
        numberpresent->setText( QString::number((*i).numberpresent));
        numberpresent->setFlags(numberpresent->flags() ^ Qt::ItemIsEditable);
        ui->periphralTable->setItem(loc, 3, numberpresent);

        loc++;
    }

    // Show next window for verification
    currentState = QSIState::VerifyWindow;
    ui->toolBox->setCurrentIndex(1);
}

void QuickScanItem::on_toolBox_currentChanged(int index)
{
    switch(currentState){
    case QSIState::ScanWindow:
        if(index != 0)
            ui->toolBox->setCurrentIndex(0);
            ui->lineEdit->setFocus();
        break;
    case QSIState::VerifyWindow:
        if(index != 1)
            ui->toolBox->setCurrentIndex(1);
        break;
    }
}

void QuickScanItem::on_addItemConfirmation_clicked()
{
    /*




        Update item's peripheral data in local context



    */

    qDebug() << "Update peripheral information here...";


    // Turn into form parent requires, and emit
    QStringList returnList;
    returnList.append(itemFound.barcode);
    emit dataReady(returnList);

    ui->errorLabel->setText("Item Added.");

    // Return to scan window
    currentState = QSIState::ScanWindow;
    ui->toolBox->setCurrentIndex(0);
}


void QuickScanItem::on_cancelButton_clicked()
{
    ui->errorLabel->setText("Cancelled.");
    currentState = QSIState::ScanWindow;
    ui->toolBox->setCurrentIndex(0);
}

void QuickScanItem::on_cancelButton_2_clicked()
{
    this->close();
}
