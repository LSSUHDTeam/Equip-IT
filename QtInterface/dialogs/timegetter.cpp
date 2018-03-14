#include "timegetter.h"
#include "ui_timegetter.h"

TimeGetter::TimeGetter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeGetter)
{
    ui->setupUi(this);
    flipmins = false;
    timeState = UberTimeGet::getStart;
    startDate = QDate::currentDate();
    startTime = QTime::currentTime();
    ui->startTimeEdit->setTime(startTime);
    ui->calendarStart->setDateRange(startDate, startDate.addYears(2));
}

TimeGetter::~TimeGetter()
{
    delete ui;
}

void TimeGetter::forceClose()
{
    this->close();
}

void TimeGetter::on_calendarStart_clicked(const QDate &date)
{
    if (timeState == UberTimeGet::getStart)
        startDate = date;
    else if (timeState == UberTimeGet::getEnd)
        endDate = date;
    updateLabel();
}

void TimeGetter::on_startTimeEdit_timeChanged(const QTime &time)
{
    if (timeState == UberTimeGet::getStart)
        startTime = time;
    else if (timeState == UberTimeGet::getEnd)
        endTime = time;

    updateLabel();

    if(time.toString("mm") == "59")
    {
        ui->errorLabel->setText("Use 'MIN' to reset minutes.");
        flipmins = true;
    }
}

void TimeGetter::on_minButton_clicked()
{
    ui->startTimeEdit->setCurrentSectionIndex(1);
    ui->errorLabel->setText("Editing 'Minute'");
    if (flipmins)
    {
        flipmins = false;
        ui->startTimeEdit->setTime(
                    QTime::fromString(
                        ui->startTimeEdit->time().toString("hh") + ":00", "hh:mm")
                    );
    }
}

void TimeGetter::on_hourButton_clicked()
{
    ui->startTimeEdit->setCurrentSectionIndex(0);
    ui->errorLabel->setText("Editing 'Hour'");
}

void TimeGetter::on_resetButton_clicked()
{
    startDate = QDate::currentDate();
    startTime = QTime::currentTime();
    ui->startTimeEdit->setTime(startTime);
    ui->calendarStart->setDateRange(startDate, startDate.addYears(2));
    flipState(UberTimeGet::getStart);
    ui->errorLabel->setText("Reset Complete");
}

void TimeGetter::on_lockInButton_clicked()
{
    switch(timeState)
    {
    case UberTimeGet::getStart:
        flipState(UberTimeGet::getEnd);
        break;
    case UberTimeGet::getEnd:
        flipState(UberTimeGet::finalize);
        break;
    case UberTimeGet::finalize:
        ui->lockInButton->setEnabled(false);
        flipState(UberTimeGet::finalize);
        break;
    }
}

void TimeGetter::updateLabel()
{
    switch(timeState)
    {
    case UberTimeGet::getStart:
        ui->errorLabel->setText(
            QDateTime(startDate, startTime).toString(
                QString(STUDENT_FRIENDLY_FORMAT_BECAUSE_THEY_DONT_UNDERSTAND_24_HOUR_TIME)
                )
            );
        break;
    case UberTimeGet::getEnd:
        ui->errorLabel->setText(
            QDateTime(endDate, endTime).toString(
                QString(STUDENT_FRIENDLY_FORMAT_BECAUSE_THEY_DONT_UNDERSTAND_24_HOUR_TIME)
                )
            );
        break;
    default:
        break;
    }
}

void TimeGetter::flipState(UberTimeGet state)
{
    timeState = state;
    switch(timeState)
    {
    case UberTimeGet::getStart:
        ui->lockInButton->setText("Set Start Time");
        ui->lockInButton->setEnabled(true);
        break;
    case UberTimeGet::getEnd:
        ui->lockInButton->setText("Set End Time");
        ui->lockInButton->setEnabled(true);
        ui->calendarStart->setMinimumDate(startDate);
        break;
    case UberTimeGet::finalize:
        finalize();
        break;
    }
}

void TimeGetter::finalize()
{
    QDateTime start = QDateTime(startDate, startTime);
    QDateTime end = QDateTime(endDate, endTime);
    if(start >= end)
    {
        startDate = QDate::currentDate();
        startTime = QTime::currentTime();
        ui->startTimeEdit->setTime(startTime);
        ui->calendarStart->setDateRange(startDate, startDate.addYears(2));
        flipState(UberTimeGet::getStart);
        ui->errorLabel->setText("Invalid Time Frame");
        return;
    }
    emit setDateTimeRange(start, end);
    this->hide();
    emit actionsCompleted();
    this->close();
}

void TimeGetter::on_amppmtoggle_clicked()
{
    ui->startTimeEdit->setTime(ui->startTimeEdit->time().addSecs(43200));
}
