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
}

BaseWindow::~BaseWindow()
{
    localContext = 0;
    delete ui;
}

void BaseWindow::performUpdateOnWindow()
{
    ui->statusbar->showMessage("User: " + localContext->getSessionUser());
}

void BaseWindow::sessionTimeout()
{
    emit isClosed();
}

void BaseWindow::updateSessionTimeRemainingOutput(QString timeRemaining)
{
    performUpdateOnWindow();
    ui->statusbar->showMessage(ui->statusbar->currentMessage() +
                               " " + timeRemaining);
}

void BaseWindow::closeEvent(QCloseEvent *event)
{
    this->close();
    event->ignore();
    localContext->killSession();
    emit isClosed();
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
