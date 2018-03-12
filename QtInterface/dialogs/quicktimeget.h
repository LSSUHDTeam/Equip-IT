#ifndef QUICKTIMEGET_H
#define QUICKTIMEGET_H

#include <QDebug>
#include <QDialog>
#include <QDateTime>
#include <QCloseEvent>

// For time formatting info
#define STUDENT_FRIENDLY_FORMAT_BECAUSE_THEY_DONT_UNDERSTAND_24_HOUR_TIME "h:mm ap : ddd MMMM d yy "

namespace Ui {
class QuickTimeGet;
}

class QuickTimeGet : public QDialog
{
    Q_OBJECT

public:
    explicit QuickTimeGet(QWidget *parent = 0);
    ~QuickTimeGet();

signals:
    void setDateTimeRange(QDateTime, QDateTime);

public slots:
    void forceClose();

private slots:
    void on_calendarWidget_clicked(const QDate &date);

    void on_timeEdit_timeChanged(const QTime &time);

    void on_completeButton_clicked();

    void on_hourButton_clicked();

    void on_minuteButton_clicked();

private:
    Ui::QuickTimeGet *ui;

    bool completed;
    bool flipmins;
    QDate endDate;
    QTime endTime;

    void updateLabel();
    void closeEvent(QCloseEvent *);
};

#endif // QUICKTIMEGET_H
