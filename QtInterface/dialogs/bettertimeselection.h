#ifndef BETTERTIMESELECTION_H
#define BETTERTIMESELECTION_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include "dialogs/manualtimeentry.h"

namespace Ui {
class BetterTimeSelection;
}

class BetterTimeSelection : public QMainWindow
{
    Q_OBJECT

public:
    explicit BetterTimeSelection(QWidget *parent = 0);
    ~BetterTimeSelection();

signals:
    void dateTimeComplete(QDateTime);

public slots:
    void manualEntryData(QString data);

private slots:
    void on_toolBox_currentChanged(int index);

    void on_nextMonth_clicked();

    void on_selectButton_clicked();

    void on_prevMonth_clicked();

private:
    Ui::BetterTimeSelection *ui;

    ManualTimeEntry * manTime;
    QMdiSubWindow *manSub;
    int properIndex;

    QDate theDate;
    QTime theTime;

    void loadMdiArea();
};

#endif // BETTERTIMESELECTION_H
