#include "quickscanitem.h"
#include "ui_quickscanitem.h"

QuickScanItem::QuickScanItem(ContextManager *context, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuickScanItem)
{
    ui->setupUi(this);

    pvalidator = new PeripheralValidator(this);
    connect(this, SIGNAL(closeChildren()), pvalidator, SLOT(forceClose()));
    connect(this, SIGNAL(updatePeriphWindow(std::vector<peripherals>)), pvalidator, SLOT(updateWindow(std::vector<peripherals>)));
    connect(pvalidator, SIGNAL(periphsValidated(std::vector<peripherals>)), this, SLOT(updatePeriphs(std::vector<peripherals>)));
    screenSub = ui->mdiArea->addSubWindow(pvalidator);
    screenSub->setWindowFlags(Qt::FramelessWindowHint);
    screenSub->showMaximized();


    this->showMaximized();
    noPeriphs = false;
    currentState = QSIState::ScanWindow;
    localContext = context;
    ui->toolBox->setCurrentIndex(0);
    ui->lineEdit->setFocus();


    // This needs to be updated to only show items available from the current time
    // to a (maximum) of MAX_FUTURE_BUFFER_MINUTES hour
    currentItems = localContext->getExistingItems();
}

QuickScanItem::~QuickScanItem()
{
    localContext = 0;
    delete pvalidator;
    delete screenSub;
    delete ui;
}

void QuickScanItem::forceClose()
{
    ui->mdiArea->removeSubWindow(pvalidator);
    emit closeChildren();
    this->close();
}

void QuickScanItem::updatePeriphs(std::vector<peripherals> p)
{
    itemFound.periphs = p;
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

    itemFound = itemScanned;

    emit updatePeriphWindow(itemFound.periphs);

    if(itemFound.periphs.size() == 0)
        noPeriphs = true;
    else
        noPeriphs = false;

    // Show next window for verification
    currentState = QSIState::VerifyWindow;
    ui->itemNameDisplayLabel->setText("Item: " + itemFound.name + ". Description: " + itemFound.desc + "\nBarcode: " + itemFound.barcode);
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
    // Update DAM item cache
    localContext->updateItemPeriphs(itemFound.barcode, itemFound.periphs);
    currentItems = localContext->getExistingItems();

    // Turn into form parent requires, and emit
    QStringList returnList;
    returnList.append(itemFound.barcode);
    emit dataReady(returnList);

    ui->errorLabel->setText("Item Added.");

    // Return to scan window
    currentState = QSIState::ScanWindow;
    ui->toolBox->setCurrentIndex(0);
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();
}


void QuickScanItem::on_cancelButton_clicked()
{
    ui->errorLabel->setText("Cancelled.");
    currentState = QSIState::ScanWindow;
    ui->toolBox->setCurrentIndex(0);
}

void QuickScanItem::on_cancelButton_2_clicked()
{
    emit reservationComplete();
    this->close();
}

void QuickScanItem::on_clearButton_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();
}
