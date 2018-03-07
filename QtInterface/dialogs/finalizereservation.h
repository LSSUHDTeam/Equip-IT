#ifndef FINALIZERESERVATION_H
#define FINALIZERESERVATION_H

#include <QMap>
#include <QDialog>
#include <QCloseEvent>
#include <QResizeEvent>
#include "framework/ephimeral.h"

namespace Ui {
class FinalizeReservation;
}

class FinalizeReservation : public QDialog
{
    Q_OBJECT

public:
    explicit FinalizeReservation(Ephimeral *currentRes, QWidget *parent = 0);
    ~FinalizeReservation();

private slots:

    void forceClose();

    void on_reportTable_clicked(const QModelIndex &index);

    void on_reportTable_doubleClicked(const QModelIndex &index);

    void on_resolveError_clicked();

    void on_completeButton_clicked();

signals:
    void forceClosed();


private:
    Ui::FinalizeReservation *ui;

    Ephimeral *ephimeral;

    bool verified, forceCancel;
    int unresolvedErrors;

    QMap<int, scheduleConflict> tableConflictMap;
    QStringList tableHeaders;
    void setupTable();
    void buildTable();

    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *);
};

#endif // FINALIZERESERVATION_H
