#ifndef RESERVATIONVIEWER_H
#define RESERVATIONVIEWER_H

#include <QDialog>
#include "framework/structures.h"
#include "framework/ephimeral.h"

namespace Ui {
class ReservationViewer;
}

class ReservationViewer : public QDialog
{
    Q_OBJECT

public:
    explicit ReservationViewer(Ephimeral *currentRes, QWidget *parent = 0);
    ~ReservationViewer();

public slots:
    void forceClose();

private slots:
    void on_pushButton_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

private:
    Ui::ReservationViewer *ui;

    Ephimeral *currentReservation;

    void populateTable();


};

#endif // RESERVATIONVIEWER_H
