#include "reservationviewer.h"
#include "ui_reservationviewer.h"

ReservationViewer::ReservationViewer(Ephimeral *currentRes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReservationViewer)
{
    currentReservation = currentRes;
    ui->setupUi(this);
    populateTable();
}

ReservationViewer::~ReservationViewer()
{
    currentReservation = 0;
    delete ui;
}

void ReservationViewer::forceClose()
{
    this->close();
}

void ReservationViewer::on_pushButton_clicked()
{
    this->close();
}

void ReservationViewer::populateTable()
{
    reservations resData = currentReservation->getCurrentReservation();
    std::vector<reservableItems> items =  currentReservation->getItemsOnReservation();

    ui->forLabel->setText(resData.wfor +
                          "(" + resData.email + ")");
    ui->timeStartLabel->setText(resData.start);
    ui->timeEndLabel->setText(resData.end);

    int count = 1;
    for(auto i = items.begin(); i != items.end(); ++i)
    {
        QString desc = (*i).desc;
        if(desc.length() > 25)
            desc = desc.mid(0,25)+"...";

        ui->listWidget->addItem(
                    "Item [" + QString::number(count++) + "]\t" +
                    (*i).barcode + "\t\t\t" + (*i).name + "\t\t\t" + desc
                    );
    }
    ui->itemCountLabel->setText(QString::number(items.size()));
}

// Not Implemented
void ReservationViewer::on_listWidget_clicked(const QModelIndex &index)
{
    Q_UNUSED(index)
}
