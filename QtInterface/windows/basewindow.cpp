#include "basewindow.h"
#include "ui_basewindow.h"

BaseWindow::BaseWindow(ContextManager * context, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BaseWindow)
{
    ui->setupUi(this);
    localContext = context;
    ui->statusbar->showMessage("");
    connect(localContext, SIGNAL(rippleSessionTimeout()), this,SLOT(sessionTimeout()));
    connect(localContext, SIGNAL(rippleSessionTicker(QString)),
            this,SLOT(updateSessionTimeRemainingOutput(QString)));
    ui->toolBox->setCurrentIndex(0);
}

BaseWindow::~BaseWindow()
{
    localContext = 0;
    delete ui;
}

/*

    Exits for base window

*/
void BaseWindow::shutdownWindow()
{
    emit forceCloseChildren();
    ui->toolBox->setEnabled(true);
    ui->toolBox->setCurrentIndex(0);
    localContext->changeUserLocation(WindowDescriptors::LoginWindow);
    emit isClosed();
}

void BaseWindow::sessionTimeout()
{
    shutdownWindow();
}

/*

    Opening and closing of child windows

*/
void BaseWindow::accessChildWindow(WindowDescriptors window)
{
    switch(window){

    /*
        Instant Checkout
    */
    case WindowDescriptors::CheckoutRoot:
    {
        localContext->changeUserLocation(WindowDescriptors::CheckoutRoot);
        CheckOut *checkout = new CheckOut(localContext, CheckoutType::QuickCheckout, this);
        connect(checkout, SIGNAL(checkoutClosed(WindowDescriptors)),
                this, SLOT(childWindowClosed(WindowDescriptors)));
        connect(this, SIGNAL(forceCloseChildren()), checkout, SLOT(forceClose()));
        checkout->setAttribute(Qt::WA_DeleteOnClose, true);
        checkout->show();
        break;
    }

    /*
        Add/Remove reservation root
    */
    case WindowDescriptors::ReservationRoot:
    {
        localContext->changeUserLocation(WindowDescriptors::ReservationRoot);
        ReservationRoot *resroot = new ReservationRoot(localContext, this);
        connect(resroot, SIGNAL(reservationRootClosed(WindowDescriptors)),
                this, SLOT(childWindowClosed(WindowDescriptors)));
        connect(this, SIGNAL(forceCloseChildren()), resroot, SLOT(forceClose()));
        resroot->setAttribute(Qt::WA_DeleteOnClose, true);
        resroot->showMaximized();
        break;
    }

    /*
        Check in
    */
    case WindowDescriptors::CheckInRoot:
    {
        localContext->changeUserLocation(WindowDescriptors::CheckInRoot);
        break;
    }

    /*
        Item overview
    */
    case WindowDescriptors::OverviewRoot:
    {
        localContext->changeUserLocation(WindowDescriptors::OverviewRoot);
        break;
    }

    /*
        Administration Panel
    */
    case WindowDescriptors::AdminPanel:
    {
        localContext->changeUserLocation(WindowDescriptors::AdminPanel);

        AdminPanel *admin = new AdminPanel(localContext,this);
        connect(admin, SIGNAL(adminPanelClosed(WindowDescriptors)),
                this, SLOT(childWindowClosed(WindowDescriptors)));
        connect(this, SIGNAL(forceCloseChildren()), admin, SLOT(requestForceClose()));
        admin->setAttribute(Qt::WA_DeleteOnClose, true);
        admin->show();

        break;
    }
    default:
        qDebug() << "basewindow->accessChildWindow() has hit a default switch-case.";
        break;
    }
    ui->toolBox->setEnabled(false);
}

void BaseWindow::childWindowClosed(WindowDescriptors window)
{
    switch(window){
    case WindowDescriptors::CheckoutRoot:
        localContext->addUserCrumb("Close Instant Checkout");
        break;
    case WindowDescriptors::ReservationRoot:
        localContext->addUserCrumb("Close Reservation Root");
        break;
    case WindowDescriptors::CheckInRoot:
        localContext->addUserCrumb("Close Check In Root");
        break;
    case WindowDescriptors::OverviewRoot:
        localContext->addUserCrumb("Close Overview");
        break;
    case WindowDescriptors::AdminPanel:
        localContext->addUserCrumb("Close Admin Panel");
        break;
    default:
        qDebug() << "basewindow->childWindowClosed() has hit a default switch-case.";
        break;
    }
    localContext->changeUserLocation(WindowDescriptors::BaseWindow);
    ui->toolBox->setEnabled(true);
}

/*

    Window events and updates

*/
void BaseWindow::closeEvent(QCloseEvent *event)
{
    this->close();
    event->ignore();
    localContext->killSession();
    shutdownWindow();
}

void BaseWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    double wratio = .27751479;
    double hratio = .27605633;
    int current_window_width = this->window()->width();
    int current_window_height = this->window()->height();
    QSize buttonSize(
                current_window_width*wratio, current_window_height*hratio
                );
    ui->checkoutButton->setMinimumSize(buttonSize);
    ui->checkInButton->setMinimumSize(buttonSize);
    ui->addRemButton->setMinimumSize(buttonSize);
    ui->overviewButton->setMinimumSize(buttonSize);
}

void BaseWindow::performUpdateOnWindow()
{
    ui->statusbar->showMessage("User: " + localContext->getSessionUser());
}

void BaseWindow::updateSessionTimeRemainingOutput(QString timeRemaining)
{
    performUpdateOnWindow();
    ui->statusbar->showMessage(ui->statusbar->currentMessage() +
                               " " + timeRemaining);
}

/*

    Buttons

*/
void BaseWindow::on_checkoutButton_clicked()
{
    accessChildWindow(WindowDescriptors::CheckoutRoot);
}

void BaseWindow::on_addRemButton_clicked()
{
    accessChildWindow(WindowDescriptors::ReservationRoot);
}

void BaseWindow::on_checkInButton_clicked()
{
    accessChildWindow(WindowDescriptors::CheckInRoot);
}

void BaseWindow::on_overviewButton_clicked()
{
   accessChildWindow(WindowDescriptors::OverviewRoot);
}

/*

    Admin panel authenticator

*/

void BaseWindow::on_toolBox_currentChanged(int index)
{
    Q_UNUSED(index)
    ui->buff->clear();
    ui->loginOutput->clear();
    ui->loginOutput->insertPlainText("> Welcome, "+localContext->getSessionUser()+". Use the panel to login...");
    adminLoginBuffer = "";
}

void BaseWindow::on_authenticate_clicked()
{
    QString buf= adminLoginBuffer;
    if (buf.length() == 0)
    {
        ui->statusbar->showMessage("No input to attempt authentication.");
        return;
    }
    ui->statusbar->showMessage("Attempting authentication.");
    ui->buff->clear();
    adminLoginBuffer.clear();
    ui->loginOutput->insertPlainText("\n> Logging authentication attempt....");
    localContext->addUserCrumb("Attempting to login to admin panel");
    ui->loginOutput->insertPlainText("\n> Generating cryptographic hashes....");

    QByteArray localData;
    QString hashFile = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/resources/ahash.eqpt";
    QByteArray tdata = QCryptographicHash::hash(buf.toLocal8Bit(), QCryptographicHash::Sha256);

    QFile local(hashFile);
    if(local.open(QIODevice::ReadOnly))
        localData = local.readAll();
    local.close();

    ui->loginOutput->insertPlainText("\n> "+QString(localData).simplified());
    ui->loginOutput->insertPlainText("\n> "+QString(tdata).simplified());
    ui->loginOutput->insertPlainText("\n\n");

    QScrollBar * sb = ui->loginOutput->verticalScrollBar();
    sb->setValue(sb->maximum());

    if( localData == tdata)
    {
        ui->loginOutput->insertPlainText("\n> Access Granted\n");
        localContext->addUserCrumb("Access to admin panel granted");

        accessChildWindow(WindowDescriptors::AdminPanel);

        ui->statusbar->showMessage("Access Granted");
        sb->setValue(sb->maximum());

    } else {
        ui->loginOutput->insertPlainText("\n> Access Denied\n");
        localContext->addUserCrumb("Access to admin panel denied");
        ui->statusbar->showMessage("Access Denied");
        sb->setValue(sb->maximum());
    }
}

void BaseWindow::addToBuff(QString item)
{
    ui->buff->insert("*");
    adminLoginBuffer.append(item);
    ui->statusbar->showMessage("Inserted: "+item+".");
}

void BaseWindow::on_clearButton_clicked()
{
    ui->buff->clear();
    ui->statusbar->showMessage("Input cleared.");
    localContext->addUserCrumb("Password Field Cleared");
}

void BaseWindow::on_r0c0_clicked()
{
    addToBuff("1");
}

void BaseWindow::on_r0c1_clicked()
{
    addToBuff("2");
}

void BaseWindow::on_r0c2_clicked()
{
    addToBuff("3");
}

void BaseWindow::on_r0c3_clicked()
{
    addToBuff("4");
}

void BaseWindow::on_r1c0_clicked()
{
    addToBuff("5");
}

void BaseWindow::on_r1c1_clicked()
{
    addToBuff("6");
}

void BaseWindow::on_r1c2_clicked()
{
    addToBuff("7");
}

void BaseWindow::on_r1c3_clicked()
{
    addToBuff("8");
}

void BaseWindow::on_r2c0_clicked()
{
    addToBuff("9");
}

void BaseWindow::on_r2c1_clicked()
{
    addToBuff("0");
}

void BaseWindow::on_r2c2_clicked()
{
    addToBuff("A");
}

void BaseWindow::on_r2c3_clicked()
{
    addToBuff("B");
}

void BaseWindow::on_r3c0_clicked()
{
    addToBuff("C");
}

void BaseWindow::on_r3c1_clicked()
{
    addToBuff("D");
}

void BaseWindow::on_r3c2_clicked()
{
    addToBuff("E");
}

void BaseWindow::on_r3c3_clicked()
{
    addToBuff("F");
}
