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
    ephimeralReservation = new Ephimeral(localContext);

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
    delete ephimeralReservation;
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
        connect(this, SIGNAL(closeChildren()), quickScan, SLOT(forceClose()));
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
    foreach(QString barcode, barcodes)
    {
        ephimeralReservation->addItemToReservation(barcode);
    }
}

/*
    Set the time frame from one of the windows
*/
void CheckOut::setTimeFrame(QDateTime start, QDateTime end)
{
    localContext->addUserCrumb("Time set to: " +
                               end.toString(
                                   QString(STUDENT_FRIENDLY_FORMAT_BECAUSE_THEY_DONT_UNDERSTAND_24_HOUR_TIME)));
     ui->displayLabelTime->setText(
                 "Returns: "+
                 end.toString(
                     QString(STUDENT_FRIENDLY_FORMAT_BECAUSE_THEY_DONT_UNDERSTAND_24_HOUR_TIME)
                     ));
     ephimeralReservation->setReservationTimeRange(start, end);
}
/*
    Set the location of the reservation
*/
void CheckOut::setBuildingAndRoom(QString building, QString room)
{
    localContext->addUserCrumb("Selected building: " + building + ", in room: " + room);
    ephimeralReservation->setReservationLocation(building, room);
    ui->diplayLabelWhere->setText(building + ", " + room);
}

/*

    Location management

*/
void CheckOut::on_locationButton_clicked()
{
    localContext->addUserCrumb("Selecting building");
    BuildingSelection *bldg = new BuildingSelection(this);
    connect(this, SIGNAL(closeChildren()), bldg, SLOT(forceClose()));
    connect(bldg, SIGNAL(buildingAndRoomSelected(QString,QString)), this, SLOT(setBuildingAndRoom(QString,QString)));
    bldg->setAttribute(Qt::WA_DeleteOnClose, true);
    bldg->showMaximized();
}

/*

    Time management

*/
void CheckOut::on_timeButton_clicked()
{

    switch(checkoutMode){
    case CheckoutType::BuilderCheckout:
    {
        localContext->addUserCrumb("Checkout 'builder' mode");
        TimeGetter *timeget = new TimeGetter(this);
        timeget->setAttribute(Qt::WA_DeleteOnClose, true);
        timeget->showMaximized();
        break;
    }
    case CheckoutType::QuickCheckout:
    {
        localContext->addUserCrumb("Getting time 'quickly'");
        QuickTimeGet *timeget = new QuickTimeGet(this);
        connect(this, SIGNAL(closeChildren()), timeget, SLOT(forceClose()));
        connect(timeget, SIGNAL(setDateTimeRange(QDateTime, QDateTime)), this, SLOT(setTimeFrame(QDateTime, QDateTime)));
        timeget->setAttribute(Qt::WA_DeleteOnClose, true);
        timeget->showMaximized();
        break;
    }
    }
}

/*

    Cancel Reservation

*/
void CheckOut::on_cancelReservationButton_clicked()
{
    localContext->addUserCrumb("Cancelling reservation");
    this->close();
}

/*

    Complete Reservation

*/
void CheckOut::on_completeReservationButton_clicked()
{
    localContext->addUserCrumb("Attempting to complete reservation");
    qDebug() << "VERIFY DETAILS AND CHECK FOR ANY TIME ISSUES";
}
