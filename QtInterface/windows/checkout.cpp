#include "checkout.h"
#include "ui_checkout.h"

CheckOut::CheckOut(ContextManager *context, CheckoutType mode, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CheckOut)
{
    // UI
    ui->setupUi(this);
    this->showMaximized();
    timeSet = false; itemsSet = false;
    forSet = false; bySet = false, allowView = false;

    // Operation dependencies
    localContext = context;
    checkoutMode = mode;
    ephimeralReservation = new Ephimeral(localContext, EphimeralStage::BuildingReservation, this);

    // Response to validation checking
    connect(ephimeralReservation, SIGNAL(invalidReservation(std::vector<scheduleConflict>)),
            this, SLOT(reservationIsInvalid(std::vector<scheduleConflict>)));
    connect(ephimeralReservation, SIGNAL(validReservation()),
            this, SLOT(reservationIsValid()));
    connect(ephimeralReservation, SIGNAL(userMarkedIgnoreNetworkErrors()),
            this, SLOT(ephimeralNetworkErrorMarkedIgnore()));
    connect(ephimeralReservation, SIGNAL(submitSuccess()),
            this, SLOT(showSubmitSuccess()));

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
    screenBoard->setAttribute(Qt::WA_DeleteOnClose, true);

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

void CheckOut::ephimeralNetworkErrorMarkedIgnore()
{
    ui->errorLabel->setText("Network error ignored.");
    localContext->addUserCrumb("User ignored network errors from ephimeral");
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

void CheckOut::checkForViewEnable()
{
    if(forSet && bySet && itemsSet && timeSet)
    {
        allowView = true;
        ui->completeReservationButton->setEnabled(true);
    }
}

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
        forSet = true;
        ephimeralReservation->setReservationFor(ui->forEdit->text());
        checkForViewEnable();
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
        bySet = true;
        checkForViewEnable();
        ephimeralReservation->setReservationEmail(ui->emailEdit->text());
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
    checkForViewEnable();
    if (allowView)
    {
        ReservationViewer *rview = new ReservationViewer(ephimeralReservation, this);
        connect(this, SIGNAL(closeChildren()), rview, SLOT(forceClose()));
        rview->setAttribute(Qt::WA_DeleteOnClose, true);
        rview->showMaximized();
    } else {
        ui->errorLabel->setText("Reservation not fully setup.");
    }
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
    itemsSet = true;
    checkForViewEnable();
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
     timeSet = true;
     checkForViewEnable();
}
/*
    Set the location of the reservation
*/
void CheckOut::setBuildingAndRoom(QString building, QString room)
{
    localContext->addUserCrumb("Selected building: " + building + ", in room: " + room);
    ephimeralReservation->setReservationLocation(building, room);

    if(building.length() > 6)
        ui->diplayLabelWhere->setText(building.mid(0,6) + "... , " + room);
    else
        ui->diplayLabelWhere->setText(building + ", " + room);
}

void CheckOut::reservationCompletedAndAcknowledged()
{
    this->close();
}

void CheckOut::finalizeReservationForceClosed()
{
    localContext->addUserCrumb("Finalizer was force closed", true);
}

void CheckOut::finalizerEditedReservation()
{
    localContext->addUserCrumb("Finalizer edited reservation, rechecking reservation.");

    if(ephimeralReservation->getItemsOnReservation().size() > 0)
    {
        ephimeralReservation->finalizeReservation();
    }
    else
    {
        itemsSet = false;
        ui->statusbar->showMessage("Reservation no longer contains any items.");
        ui->completeReservationButton->setEnabled(false);
    }
}

void CheckOut::showSubmitSuccess()
{
    localContext->addUserCrumb("Valid reservation created");
    QStringList message;
    message << "\n\n\tThe reservation request has been posted to the server! " <<
               "Once you close this message (OKAY), you will be returned to the " <<
               "primary Equip-IT window.";
    SimpleMessageBox *smb = new SimpleMessageBox(smbdata("Checkout", "Valid Reservation", message), this);
    connect(this, SIGNAL(closeChildren()), smb, SLOT(forceClose()));
    connect(smb, SIGNAL(messageBoxClosed()), this, SLOT(reservationCompletedAndAcknowledged()));
    smb->setAttribute(Qt::WA_DeleteOnClose, true);
    smb->show();
}

/*

    Location management

*/
void CheckOut::on_locationButton_clicked()
{
    localContext->addUserCrumb("Selecting building");
    BuildingSelection *bldg = new BuildingSelection(localContext, this);
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
    localContext->addUserCrumb("Requesting ephimeral validation");
    ephimeralReservation->finalizeReservation();
}

void CheckOut::reservationIsValid()
{
    localContext->addUserCrumb("Reservation deemed valid, pushing to server");
    ephimeralReservation->submitCompletedReservation();
}

void CheckOut::reservationIsInvalid(std::vector<scheduleConflict> schedulingConflicts)
{
    localContext->addUserCrumb(QString::number(schedulingConflicts.size()) +
                               " scheduling conflicts were found.");
    FinalizeReservation *fr = new FinalizeReservation(ephimeralReservation, this);
    connect(this, SIGNAL(closeChildren()), fr, SLOT(forceClose()));
    connect(fr, SIGNAL(forceClosed()), this, SLOT(finalizeReservationForceClosed()));
    connect(fr, SIGNAL(conflictFlipped()), this, SLOT(finalizerEditedReservation()));
    fr->setAttribute(Qt::WA_DeleteOnClose, true);
    fr->showMaximized();
    localContext->addUserCrumb("Reservation conflict manager opened.");
}

// Testing is cool i guess.
void CheckOut::on_testLoadingButton_clicked()
{
    QDateTime start = QDateTime::currentDateTime();
    QDateTime end = start.addDays(1);

    ephimeralReservation->addItemToReservation("938-x837-3284");
    ephimeralReservation->addItemToReservation("929-x837-3284");
    ephimeralReservation->setReservationTimeRange(start, end);
    ephimeralReservation->setReservationFor("Josh Tester");
    ephimeralReservation->setReservationEmail("jbosley2@lssu.edu");
    ui->completeReservationButton->setEnabled(true);
}
