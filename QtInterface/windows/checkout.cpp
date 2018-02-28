#include "checkout.h"
#include "ui_checkout.h"

CheckOut::CheckOut(ContextManager *context, CheckoutType mode, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CheckOut)
{
    // UI
    ui->setupUi(this);
    this->showMaximized();

    // Operation dependencies
    localContext = context;
    checkoutMode = mode;

    // Log current mode
    switch(checkoutMode){
    case CheckoutType::BuilderCheckout:
        localContext->addUserCrumb("Checkout 'builder' mode");
        break;
    case CheckoutType::QuickCheckout:
        localContext->addUserCrumb("Checkout 'quick' mode");
        break;
    }

    /*
        Setup custom touchable line edits
    */
    connect(ui->forEdit, SIGNAL(focusGained()), this, SLOT(forFocusGained()));
    connect(ui->forEdit, SIGNAL(focusLoss()), this, SLOT(forFocusLost()));
    connect(ui->emailEdit, SIGNAL(focusGained()), this, SLOT(emailFocusGained()));
    connect(ui->emailEdit, SIGNAL(focusLoss()), this, SLOT(emailFocusLost()));

    /*
        Setup on-screen keyboard window
    */
    keyDirector = KeyboardFlow::ignore;
    screenBoard = new ScreenBoard(this);
    connect(this, SIGNAL(closeKeyboard()), screenBoard, SLOT(forceClose()));
    connect(screenBoard, SIGNAL(windowClosing()), this, SLOT(keyboardClosed()));
    connect(screenBoard, SIGNAL(keyPress(QString)), this, SLOT(keyboardDataReceived(QString)));

    /*
        Setup MDI area that contains on-screen keyboard
    */
    screenSub = ui->mdiArea->addSubWindow(screenBoard);
    screenSub->setWindowFlags(Qt::FramelessWindowHint);
    screenSub->showMaximized();

    ui->forEdit->setStyleSheet("QLineEdit { background: rgb(102, 153, 153); selection-background-color: rgb(102, 204, 255); }");
    ui->emailEdit->setStyleSheet("QLineEdit { background: rgb(102, 153, 153); selection-background-color: rgb(102, 204, 255); }");
}

/*
    Basic window operations
*/
CheckOut::~CheckOut()
{
    localContext = 0;
    delete screenSub;
    delete screenBoard;
    delete ui;
}

void CheckOut::shutdownWindow()
{
    ui->mdiArea->removeSubWindow(screenBoard);
    localContext->addUserCrumb("Checkout closed");
    emit closeChildren();
    emit closeKeyboard();
    emit checkoutClosed(WindowDescriptors::CheckoutRoot);
    this->close();
}

void CheckOut::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    shutdownWindow();
}

// External signal to shutdown window (session end, etc)
void CheckOut::forceClose()
{
    shutdownWindow();
}

/*
    Handle signals from custom LineEdit
*/

void CheckOut::forFocusGained()
{
    ui->emailEdit->setStyleSheet("QLineEdit { background: rgb(102, 153, 153); selection-background-color: rgb(102, 204, 255); }");
    ui->forEdit->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(102, 204, 255); }");
    localContext->addUserCrumb("Editing checkout 'for'");
    keyDirector = KeyboardFlow::who;
}


void CheckOut::emailFocusGained()
{
    ui->forEdit->setStyleSheet("QLineEdit { background: rgb(102, 153, 153); selection-background-color: rgb(102, 204, 255); }");
    ui->emailEdit->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); selection-background-color: rgb(102, 204, 255); }");
    localContext->addUserCrumb("Editing Email");
    keyDirector = KeyboardFlow::email;
}

void CheckOut::forFocusLost(){}
void CheckOut::emailFocusLost(){}

/*

    On-screen keyboard

*/

// No longer implemented - keyboard remains open
void CheckOut::bringUpKeyboard(){}

void CheckOut::keyboardClosed()
{
    localContext->addUserCrumb("Keyboard closed");
}

void CheckOut::keyboardDataReceived(QString data)
{
    qDebug() << data;
    switch(keyDirector){
    case KeyboardFlow::who:

        if (data == QString(SCREENBOARD_BACKSPACE))
        {
            QString fedit = ui->forEdit->text();
            fedit = fedit.left(fedit.length()-1);
            ui->forEdit->setText(fedit);
        }
        else
            ui->forEdit->insert(data);
        break;
    case KeyboardFlow::email:
        if (data == QString(SCREENBOARD_BACKSPACE))
        {
            QString fedit = ui->emailEdit->text();
            fedit = fedit.left(fedit.length()-1);
            ui->emailEdit->setText(fedit);
        }
        else
            ui->emailEdit->insert(data);
        break;
    case KeyboardFlow::ignore:
        qDebug() << "Keyboard data " << data << " present while ignoring.\n";
        break;
    default:
        qDebug() << "Keyboard data " << data << " present in default case.\n";
        break;
    }
}

/*

    Items builder management

*/
void CheckOut::on_viewItemsButton_clicked()
{

}

void CheckOut::on_addItemButton_clicked()
{
    switch(checkoutMode){
    case CheckoutType::BuilderCheckout:
        localContext->addUserCrumb("Checkout 'builder' mode");
        break;
    case CheckoutType::QuickCheckout:
    {

        QuickScanItem *quickScan = new QuickScanItem(localContext, this);
        connect(this, SIGNAL(closeChildren()), quickScan, SLOT(foceClose()));
        connect(quickScan, SIGNAL(dataReady(QStringList)), this, SLOT(addItemsToReservation(QStringList)));
        quickScan->setAttribute(Qt::WA_DeleteOnClose, true);
        quickScan->show();
        break;
    }
    }
}

/*
    Data retrieval window slots
*/
void CheckOut::addItemsToReservation(QStringList barcodes)
{

}

/*

    Location management

*/
void CheckOut::on_locationButton_clicked()
{

}

/*

    Time management

*/
void CheckOut::on_timeButton_clicked()
{

}
