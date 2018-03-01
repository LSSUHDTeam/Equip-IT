#ifndef RESERVATIONVIEWER_H
#define RESERVATIONVIEWER_H

#include <QMainWindow>

namespace Ui {
class ReservationViewer;
}

class ReservationViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReservationViewer(QWidget *parent = 0);
    ~ReservationViewer();

private:
    Ui::ReservationViewer *ui;
};

#endif // RESERVATIONVIEWER_H
