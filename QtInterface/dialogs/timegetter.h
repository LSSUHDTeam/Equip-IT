#ifndef TIMEGETTER_H
#define TIMEGETTER_H

#include <QDialog>
#include <QDate>
#include <QTime>
#include <QString>
#include <QDateTime>
#include "framework/structures.h"
#include "dialogs/manualtimeentry.h"
#include "windows/configurerepetition.h"

#define STUDENT_FRIENDLY_FORMAT_BECAUSE_THEY_DONT_UNDERSTAND_24_HOUR_TIME "h:mm ap : ddd MMMM d yy "


enum class UberTimeGet{
    getStart,
    getEnd,
    finalize
};

namespace Ui {
class TimeGetter;
}

class TimeGetter : public QDialog
{
    Q_OBJECT

public:
    explicit TimeGetter(QWidget *parent = 0);
    ~TimeGetter();

signals:
    closeChildren();
    setDateTimeRange(QDateTime, QDateTime);
    generateRepeatedSchedule(repetition);
    actionsCompleted();

public slots:
    void forceClose();

    void manual_time_returned(QString);

    void repeat_selection_returned(repetition data);

private slots:
    void on_calendarStart_clicked(const QDate &date);

    void on_startTimeEdit_timeChanged(const QTime &time);

    void on_minButton_clicked();

    void on_hourButton_clicked();

    void on_resetButton_clicked();

    void on_lockInButton_clicked();

    void on_amppmtoggle_clicked();

    void on_prevMonthButton_clicked();

    void on_nextMonthButton_clicked();

    void on_manualTimeEntry_clicked();

    void on_repeatReservation_clicked();

private:
    Ui::TimeGetter *ui;
    UberTimeGet timeState;
    bool flipmins;
    QDate endDate, startDate;
    QTime endTime, startTime;

    void updateLabel();
    void flipState(UberTimeGet state);
    void finalize();
};

#endif // TIMEGETTER_H
