#include "peripheralvalidator.h"
#include "ui_peripheralvalidator.h"

PeripheralValidator::PeripheralValidator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PeripheralValidator)
{
    ui->setupUi(this);
    this->showMaximized();
    peripheralList << "Name" << "Present";
    ui->periphralTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

PeripheralValidator::~PeripheralValidator()
{
    delete ui;
}

void PeripheralValidator::forceClose()
{
    this->close();
}

void PeripheralValidator::updateWindow(std::vector<peripherals> iperiphs)
{
    periphs.clear();

    // We emit back the initally passed-in data to ensure that
    // the data is registered with the parent - Should be, but
    // since this window can be called by anyone, its best to
    // be safe
    emit periphsValidated(iperiphs);

    // Wrap the peripherals to manage the displaying and editing
    // if the perihperal data
    for(auto i = iperiphs.begin(); i != iperiphs.end(); ++i)
    {
        for(int j = 0; j != (*i).numberpresent; j++)
        {
            periphs.push_back(PeriphScreenWrapper((*i)));
        }
    }
    setupTable();
}

void PeripheralValidator::setupTable()
{
    ui->periphralTable->clear();
    ui->periphralTable->setColumnCount(peripheralList.length());
    ui->periphralTable->setHorizontalHeaderLabels(peripheralList);
    ui->periphralTable->setColumnWidth(0, this->width()/2);
    ui->periphralTable->setColumnWidth(1, this->width()/2);
    ui->periphralTable->setRowCount(periphs.size());
    buildLists();
}

void PeripheralValidator::buildLists()
{
    int loc = 0;
    for(auto i = periphs.begin(); i != periphs.end(); ++i)
    {
        QTableWidgetItem *name = new QTableWidgetItem();
        name->setText((*i).periph.name);
        name->setFlags(name->flags() ^ Qt::ItemIsEditable);
        ui->periphralTable->setItem(loc, 0, name);
        QTableWidgetItem *present = new QTableWidgetItem();
        if ((*i).present)
            present->setText("PRESENT");
        else
            present->setText("NOT PRESENT");

        present->setFlags(present->flags() ^ Qt::ItemIsEditable);
        ui->periphralTable->setItem(loc, 1, present);
        ui->periphralTable->setRowHeight(loc, 80);
        loc++;
    }
}

void PeripheralValidator::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    setupTable();
}

void PeripheralValidator::on_periphralTable_cellClicked(int row, int column)
{
    Q_UNUSED(column)

    // Toggle 'present' value and show
    (periphs[row].present) ? periphs[row].present = false : periphs[row].present = true;
    setupTable();
    ui->periphralTable->selectRow(row);


    // Un-wrap the peripheral data back into the required
    // DB form for the parent
    std::vector<peripherals> parsed;
    for(auto i = periphs.begin(); i != periphs.end(); ++i)
    {
        bool innerloc = false;
        for(auto j = parsed.begin(); j != parsed.end() && !innerloc; ++j)
        {
            if((*i).periph == (*j) )
                innerloc = true;
        }
        if (!innerloc)
            parsed.push_back((*i).periph);
    }

    for(auto i = periphs.begin(); i != periphs.end(); ++i)
    {
        for(auto j = parsed.begin(); j != parsed.end(); ++j)
        {
            if((*i).periph == (*j) )
            {
                if((*i).present)
                {
                    if((*j).numberpresent < (*j).count)
                        (*j).numberpresent++;
                } else {
                    if((*j).numberpresent > 0)
                        (*j).numberpresent--;
                }
            }
        }
    }

    // Send the new data away
    emit periphsValidated(parsed);
}
