#include "reservationviewer.h"
#include "ui_reservationviewer.h"

ReservationViewer::ReservationViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReservationViewer)
{
    ui->setupUi(this);
}

ReservationViewer::~ReservationViewer()
{
    delete ui;
}
