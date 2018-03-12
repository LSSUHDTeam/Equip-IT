#ifndef ERRORRESOLVER_H
#define ERRORRESOLVER_H

#include <QMap>
#include <QMainWindow>
#include "framework/structures.h"
#include "dialogs/quicktimeget.h"

namespace Ui {
class ErrorResolver;
}

class ErrorResolver : public QMainWindow
{
    Q_OBJECT

public:
    explicit ErrorResolver(int index,
                           QMap<int, scheduleConflict> conflicts,
                           QWidget *parent = 0);
    ~ErrorResolver();

signals:
    void removeItem(int index);
    void changeEndTime(QDateTime earliestTime);
    void changeTimeFrame(QDateTime start, QDateTime end);

public slots:
    void quickTimeChanged(QDateTime s, QDateTime e);

private slots:
    void on_removeItem_clicked();

    void on_moveUpTimeButton_clicked();

    void on_timeEditButton_clicked();

private:
    Ui::ErrorResolver *ui;
    int conflictIndex;
    QDateTime earliestConflict;
    QMap<int, scheduleConflict> conflicts;

    void setupWindow();
    void configureTimeOverlap();
    void configureEndTimeOverlap();
    void configureInvalidReservation();
};

#endif // ERRORRESOLVER_H
