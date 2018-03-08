#ifndef ERRORRESOLVER_H
#define ERRORRESOLVER_H

#include <QMap>
#include <QMainWindow>
#include "framework/structures.h"

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

private slots:
    void on_removeItem_clicked();

    void on_moveUpTimeButton_clicked();

private:
    Ui::ErrorResolver *ui;
    int conflictIndex;
    QDateTime earliestConflict;
    QMap<int, scheduleConflict> conflicts;

    void setupWindow();
    void configureTimeOverlap();
    void configureEndTimeOverlap();
};

#endif // ERRORRESOLVER_H
