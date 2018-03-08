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
            QDateTime curr = QDateTime::fromString(
                        conflicts.value(key).conflict.start
                        , QString(DATETIME_FORMAT));
            curr.addSecs(MINUTES_TIME_BUFFER * 60);
            if(earliestConflict.isNull() || curr < earliestConflict)
                earliestConflict = curr;
        }
    }
    setupWindow();
}

ErrorResolver::~ErrorResolver()
{
    delete ui;
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
    ui->itemLabel->setText(conflicts[conflictIndex].itembarcode.mid(0,10));
    foreach(const QString line, desc)
        ui->descriptionText->insertPlainText(line);
    ui->moveUpTimeButton->hide();
}

void ErrorResolver::configureEndTimeOverlap()
{
    QStringList desc;
    desc << "\tThe current item's schedule indicates that the item is " <<
            "available for a portion of the requested time, but must " <<
            "be returned earlier than the requested reservation's end-time " <<
            "to ensure availability for a previously scheduled reservation." <<
            "\n\nThe time that the item must be moved up-to is ";
    ui->errorTypeLabel->setText("End Overlap");
    ui->itemLabel->setText(conflicts[conflictIndex].itembarcode.mid(0,10));
    foreach(const QString line, desc)
        ui->descriptionText->insertPlainText(line);
}

void ErrorResolver::on_removeItem_clicked()
{
    emit removeItem(conflictIndex);
    this->close();
}

void ErrorResolver::on_moveUpTimeButton_clicked()
{
    emit changeEndTime(earliestConflict.addMSecs(60));
    this->close();
}
