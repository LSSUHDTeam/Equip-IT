#include "quicktimeget.h"
#include "ui_quicktimeget.h"

QuickTimeGet::QuickTimeGet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuickTimeGet)
{
    ui->setupUi(this);
    flipmins = false;
    endDate = QDate::currentDate();
    endTime = QTime::currentTime();
    ui->timeEdit->setTime(endTime);
    ui->calendarWidget->setDateRange(endDate, endDate.addMonths(1));
    ui->timeEdit->setCurrentSectionIndex(0);
    ui->editingLabel->setText("Editing 'Hour'");
    updateLabel();
}

QuickTimeGet::~QuickTimeGet()
{
    delete ui;
}

void QuickTimeGet::forceClose()
{
    completed = true;
    this->close();
}

void QuickTimeGet::on_calendarWidget_clicked(const QDate &date)
{
    endDate = date;
    updateLabel();
}

void QuickTimeGet::on_timeEdit_timeChanged(const QTime &time)
{
    endTime = time;
    updateLabel();

    if(time.toString("mm") == "59")
    {
        ui->label->setText("Use 'MIN' to reset minutes.");
        flipmins = true;
    }
}

void QuickTimeGet::on_completeButton_clicked()
{
    QDateTime start = QDateTime(QDate::currentDate(), QTime::currentTime());
    QDateTime end = QDateTime(endDate, endTime);

    if(start >= end)
    {
        ui->label->setText("Invalid Time Frame");
        return;
    }

    completed = true;
    emit setDateTimeRange(start, end);
    this->close();
}

void QuickTimeGet::updateLabel()
{
    /*
            Literally always asked 'WHY IS THERE NO AM/PM.' 'WAHT U MEAN 05:00 ISNT WHEN WE CLOSE AT NOIGHT'
    */
    ui->label->setText(
                QDateTime(endDate, endTime).toString(
                    QString(STUDENT_FRIENDLY_FORMAT_BECAUSE_THEY_DONT_UNDERSTAND_24_HOUR_TIME)
                    )
                );
}

void QuickTimeGet::closeEvent(QCloseEvent *event)
{
    if(!completed)
        event->ignore();
    else
        event->accept();
}

void QuickTimeGet::on_hourButton_clicked()
{
    ui->timeEdit->setCurrentSectionIndex(0);
    ui->editingLabel->setText("Editing 'Hour'");
}

void QuickTimeGet::on_minuteButton_clicked()
{
    ui->timeEdit->setCurrentSectionIndex(1);
    ui->editingLabel->setText("Editing 'Minute'");
    if (flipmins)
    {
        flipmins = false;
        ui->timeEdit->setTime(QTime::fromString( ui->timeEdit->time().toString("hh") + ":00", "hh:mm" ));
    }
}
