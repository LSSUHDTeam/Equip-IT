#include "reservationroot.h"
#include "ui_reservationroot.h"

ReservationRoot::ReservationRoot(ContextManager * context, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReservationRoot)
{
    ui->setupUi(this);
    localContext = context;
    controlAvailability = true;
}

ReservationRoot::~ReservationRoot()
{
    delete ui;
}

void ReservationRoot::forceClose()
{
    shutdownWindow();
}

void ReservationRoot::childWindowClosed(WindowDescriptors wid)
{
    if(wid == WindowDescriptors::CheckoutRoot)
    {
        localContext->addUserCrumb("CheckoutRoot Closed");
    }
    else if (wid == WindowDescriptors::ReservationEdit)
    {
        localContext->addUserCrumb("ReservationEdit Closed");
    }
    else
    {
        qDebug() << "ReservationRoot::childWindowClosed: Unhandled child was closed!";
    }
    localContext->changeUserLocation(WindowDescriptors::ReservationRoot);
    toggleControlAvailabilty();
    shutdownWindow();
}

void ReservationRoot::on_newResButton_clicked()
{
    localContext->changeUserLocation(WindowDescriptors::CheckoutRoot);
    localContext->addUserCrumb("Creating new reservation");

    CheckOut *checkout = new CheckOut(localContext, CheckoutType::BuilderCheckout, this);
    connect(checkout, SIGNAL(checkoutClosed(WindowDescriptors)), this, SLOT(childWindowClosed(WindowDescriptors)));
    connect(this, SIGNAL(forceCloseChildren()), checkout, SLOT(forceClose()));
    checkout->setAttribute(Qt::WA_DeleteOnClose, true);
    checkout->showMaximized();
    toggleControlAvailabilty();
}

void ReservationRoot::on_editExistingButton_clicked()
{
    localContext->addUserCrumb("Edit existing reservation");
}

void ReservationRoot::on_cancelButton_clicked()
{
    localContext->addUserCrumb("Cancel reservation action");
    shutdownWindow();
}

void ReservationRoot::shutdownWindow()
{
    emit forceCloseChildren();
    emit reservationRootClosed(WindowDescriptors::ReservationRoot);
    this->close();
}

void ReservationRoot::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    shutdownWindow();
}

void ReservationRoot::toggleControlAvailabilty()
{
    (controlAvailability) ? controlAvailability = false : controlAvailability = true;
    ui->cancelButton->setEnabled(controlAvailability);
    ui->newResButton->setEnabled(controlAvailability);
    ui->editExistingButton->setEnabled(controlAvailability);
}
