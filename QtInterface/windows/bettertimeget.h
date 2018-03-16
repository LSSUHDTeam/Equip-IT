#ifndef BETTERTIMEGET_H
#define BETTERTIMEGET_H

#include <QMainWindow>
#include <QDebug>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QString>
#include <QDateTime>
#include "framework/structures.h"
#include "framework/ephimeral.h"

#include "dialogs/bettertimeselection.h"

namespace Ui {
class BetterTimeGet;
}

class BetterTimeGet : public QMainWindow
{
    Q_OBJECT

public:
    explicit BetterTimeGet(Ephimeral *reservation, QWidget *parent = 0);
    ~BetterTimeGet();

signals:
    void timeIndexConfigured();

public slots:
    void forceClose();
    void startTimeSet(QDateTime);
    void endTimeSet(QDateTime);
    void repetitionSet(repetition);
    void noRepetition();

private slots:
    void on_startTime_clicked();

    void on_endTime_clicked();

    void on_repConfig_clicked();

    void on_complete_clicked();


private:
    Ui::BetterTimeGet *ui;

    bool forcedDown, st, et, rep;
    Ephimeral *currentReservation;
    QMdiSubWindow *startTimeWindow;
    QMdiSubWindow *endTimeWindow;
    QMdiSubWindow *repeatWindow;
    QString selectedButton, standardButton;

    QDateTime start, end;
    repetition repetitionData;
};

#endif // BETTERTIMEGET_H
