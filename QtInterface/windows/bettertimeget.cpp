#include "bettertimeget.h"
#include "ui_bettertimeget.h"

BetterTimeGet::BetterTimeGet(Ephimeral *reservation, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BetterTimeGet)
{
    ui->setupUi(this);
    forcedDown = false;
    st = false; et = false; rep = false;

    currentReservation = reservation;
    selectedButton = "background-color: rgb(102, 102, 153); font: 12pt 'Terminal'; color: rgb(255, 255, 255)";
    standardButton = "background-color: rgb(102, 153, 153); font: 12pt 'Terminal'; color: rgb(255, 255, 255)";

    ui->startTime->setStyleSheet(standardButton);
    ui->startTime->click();
}

BetterTimeGet::~BetterTimeGet()
{
    /*
    delete startTimeWindow;
    delete endTimeWindow;
    delete repeatWindow;
    */
    delete ui;
}

void BetterTimeGet::forceClose()
{
    forcedDown = true;
    this->close();
}

void BetterTimeGet::startTimeSet(QDateTime dt)
{
    start = dt;
    st = true;

    ui->startTime->setStyleSheet(selectedButton);
    ui->endTime->setStyleSheet(standardButton);
    ui->endTime->click();
}

void BetterTimeGet::endTimeSet(QDateTime dt)
{
    end = dt;
    et = true;

    ui->endTime->setStyleSheet(selectedButton);
    ui->repConfig->setStyleSheet(standardButton);
    ui->repConfig->click();
}

void BetterTimeGet::repetitionSet(repetition rd)
{
    ui->repConfig->setStyleSheet(selectedButton);
    ui->complete->setStyleSheet(standardButton);
    repetitionData = rd;
    rep = true;
}

void BetterTimeGet::noRepetition()
{
    rep = true;
}

void BetterTimeGet::on_startTime_clicked()
{
    BetterTimeSelection * bts = new BetterTimeSelection(this);
    bts->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(bts, SIGNAL(dateTimeComplete(QDateTime)),
            this, SLOT(startTimeSet(QDateTime)));
    startTimeWindow = ui->mdiArea->addSubWindow(bts);
    startTimeWindow->setWindowFlags(Qt::FramelessWindowHint);
    startTimeWindow->showMaximized();
}

void BetterTimeGet::on_endTime_clicked()
{
    BetterTimeSelection * bts = new BetterTimeSelection(this);
    bts->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(bts, SIGNAL(dateTimeComplete(QDateTime)),
            this, SLOT(endTimeSet(QDateTime)));
    endTimeWindow = ui->mdiArea->addSubWindow(bts);
    endTimeWindow->setWindowFlags(Qt::FramelessWindowHint);
    endTimeWindow->showMaximized();
}

void BetterTimeGet::on_repConfig_clicked()
{
    /*

        Load the repetition window thing

        Make a window thats all like "Yo is this thing repeated?"

        if it isnt, we're all set.

        If it is, load the rep window thing

        Need to create way to pull from ephimral if res was setup once.
        so we can auto-load pre-configured repeated stuff.

    */
}

void BetterTimeGet::on_complete_clicked()
{
    /*

            Using local ephimeral pointer,
            set the start end time and rep info.

            Once completed, emit something to CheckOut
            to indicate completeness

            if st && et && rep

    */
}

