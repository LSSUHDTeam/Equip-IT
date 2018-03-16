#include "bettertimeselection.h"
#include "ui_bettertimeselection.h"

BetterTimeSelection::BetterTimeSelection(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BetterTimeSelection)
{
    ui->setupUi(this);
    properIndex = 0;
}

BetterTimeSelection::~BetterTimeSelection()
{
    delete ui;
}

void BetterTimeSelection::manualEntryData(QString data)
{
    qDebug() << "BetterTimeSelection::manualEntryData: " << data;
    ui->mdiArea->removeSubWindow(manTime);
    manTime->close();

    theTime = QTime::fromString(data, "h:mm AP");

    emit dateTimeComplete(QDateTime(theDate, theTime));
    this->close();
}

void BetterTimeSelection::on_toolBox_currentChanged(int index)
{
    if(index != properIndex)
    {
        ui->toolBox->setCurrentIndex(properIndex);
    }
}

void BetterTimeSelection::on_nextMonth_clicked()
{
    ui->calendarWidget->setCurrentPage(
                ui->calendarWidget->selectedDate().year(),
                ui->calendarWidget->selectedDate().month()+1);
}

void BetterTimeSelection::on_selectButton_clicked()
{
    theDate = ui->calendarWidget->selectedDate();
    loadMdiArea();
    properIndex = 1;
    ui->toolBox->setCurrentIndex(1);
}

void BetterTimeSelection::on_prevMonth_clicked()
{
    ui->calendarWidget->setCurrentPage(
                ui->calendarWidget->selectedDate().year(),
                ui->calendarWidget->selectedDate().month()-1);
}

void BetterTimeSelection::loadMdiArea()
{
    manTime = new ManualTimeEntry(this);
    connect(manTime, SIGNAL(manualEntryComplete(QString)),
            this, SLOT(manualEntryData(QString)));
    manTime->setAttribute(Qt::WA_DeleteOnClose, true);

    manSub = ui->mdiArea->addSubWindow(manTime);
    manSub->setWindowFlags(Qt::FramelessWindowHint);
    manSub->showMaximized();
}
