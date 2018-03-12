#include "errorresolver.h"
#include "ui_errorresolver.h"

ErrorResolver::ErrorResolver(int rindex, QMap<int, scheduleConflict> rconflicts, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ErrorResolver)
{
    ui->setupUi(this);
    conflictIndex = rindex;
    conflicts = rconflicts;

    // Calculate the earliest time of end-overlap conflicts
    for(auto key : conflicts.keys())
    {
        if(conflicts.value(key).conflictType == ScheduleConflictTypes::endTimeOverlap)
        {
            QDateTime temp = QDateTime::fromString(
                        conflicts.value(key).conflict.start
                        , QString(DATETIME_FORMAT));

            QDateTime curr = temp.addSecs(-MINUTES_TIME_BUFFER*60);

            if(earliestConflict.isNull() || curr < earliestConflict)
                earliestConflict = curr;

            qDebug() << conflicts.value(key).conflict.start << " => " <<
                        curr.toString(QString(DATETIME_FORMAT));

        }
    }
    setupWindow();
}

ErrorResolver::~ErrorResolver()
{
    delete ui;
}

void ErrorResolver::quickTimeChanged(QDateTime s, QDateTime e)
{
    emit changeTimeFrame(s, e);
    this->close();
}

void ErrorResolver::setupWindow()
{
    switch(conflicts[conflictIndex].conflictType)
    {
    case ScheduleConflictTypes::startTimeOverlap:
        configureTimeOverlap();
        break;
    case ScheduleConflictTypes::endTimeOverlap:
        configureEndTimeOverlap();
        break;
    case ScheduleConflictTypes::invalidReservationTime:
        configureInvalidReservation();
        break;
    }
}

void ErrorResolver::configureTimeOverlap()
{
    QStringList desc;
    desc << "\tThe current item's schedule indicates that it is reserved " <<
            "for the time index of the requested reservation. This means " <<
            "that the item will gone during the time that the requested " <<
            "reservation is set to start. The only potential course of " <<
            "action that can be taken at this time is to remove it from the " <<
            "current reservation.";
    ui->errorTypeLabel->setText("Start Overlap");
    ui->itemLabel->setText(conflicts[conflictIndex].itembarcode.mid(0,20));
    foreach(const QString line, desc)
        ui->descriptionText->insertPlainText(line);
    ui->moveUpTimeButton->hide();
    ui->timeEditButton->hide();
}

void ErrorResolver::configureEndTimeOverlap()
{
    QStringList desc;
    desc << "\tThe current item's schedule indicates that the item is " <<
            "available for a portion of the requested time, but must " <<
            "be returned earlier than the requested reservation's end-time " <<
            "to ensure availability for a previously scheduled reservation." <<
            "\n\nThe time that the item must be moved up-to is \n\n\t\t\t" <<
            earliestConflict.toString(QString(DATETIME_FORMAT)) <<
            "\n\nIf this is not enough time for the reservation, please remove the item.";

    ui->errorTypeLabel->setText("End Overlap");
    ui->itemLabel->setText(conflicts[conflictIndex].itembarcode.mid(0,20));
    foreach(const QString line, desc)
        ui->descriptionText->insertPlainText(line);
    ui->timeEditButton->hide();
}

void ErrorResolver::configureInvalidReservation()
{
    QStringList desc;
    desc << "\tThe reservation was somehow corrupted, and the time range has " <<
            "become invalid. This means that the start and end time are configured " <<
            "in a nonsensical way (start and end times are the same, or end time is " <<
            "before start time.) This is an edge case that can be raised while attempting " <<
            "to resolve other errors.\n\n" <<
            "You can choose to remove this item, or if the conflict has been raised " <<
            "by multiple items, you can manually change the reservation's time frame using " <<
            "the 'Launch Time Editor' button.";

    ui->errorTypeLabel->setText("Misconfigured Time Range");
    ui->itemLabel->setText(conflicts[conflictIndex].itembarcode.mid(0,20));
    foreach(const QString line, desc)
        ui->descriptionText->insertPlainText(line);
    ui->moveUpTimeButton->hide();
}

void ErrorResolver::on_removeItem_clicked()
{
    emit removeItem(conflictIndex);
    this->close();
}

void ErrorResolver::on_moveUpTimeButton_clicked()
{
    emit changeEndTime(earliestConflict);
    this->close();
}

void ErrorResolver::on_timeEditButton_clicked()
{
    QuickTimeGet *timeget = new QuickTimeGet(this);
    connect(timeget, SIGNAL(setDateTimeRange(QDateTime, QDateTime)), this, SLOT(quickTimeChanged(QDateTime, QDateTime)));
    timeget->setAttribute(Qt::WA_DeleteOnClose, true);
    timeget->showMaximized();
}
