#include "finalizereservation.h"
#include "ui_finalizereservation.h"

FinalizeReservation::FinalizeReservation(Ephimeral *currentRes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FinalizeReservation)
{
    ui->setupUi(this);
    verified = false;
    forceCancel = false;
    unresolvedErrors = 0;
    ephimeral = currentRes;
    tableHeaders << "Item Name" << "Barcode" << "Conflict Type";
    setupTable();
}

FinalizeReservation::~FinalizeReservation()
{
    ephimeral = 0;
    delete ui;
}

void FinalizeReservation::forceClose()
{
    forceCancel = true;
    this->close();
}

void FinalizeReservation::setupTable()
{
    tableConflictMap.clear();
    ui->reportTable->clear();
    ui->reportTable->setColumnCount(tableHeaders.length());
    ui->reportTable->setHorizontalHeaderLabels(tableHeaders);
    ui->reportTable->setColumnWidth(0, this->width()/2);
    ui->reportTable->setColumnWidth(1, this->width()/2);
    ui->reportTable->setRowCount(ephimeral->numberOfConflicts());
    buildTable();
}

void FinalizeReservation::buildTable()
{
    std::vector<scheduleConflict> conflicts = ephimeral->getReservationConflicts();

    int loc = 0;
    for(auto i = conflicts.begin(); i != conflicts.end(); ++i)
    {
        QTableWidgetItem *name = new QTableWidgetItem();
        name->setText(ephimeral->getItemNameFromBarcode((*i).itembarcode));
        name->setFlags(name->flags() ^ Qt::ItemIsEditable);
        ui->reportTable->setItem(loc, 0, name);

        QTableWidgetItem *bc = new QTableWidgetItem();
        bc->setText((*i).itembarcode);
        bc->setFlags(bc->flags() ^ Qt::ItemIsEditable);
        ui->reportTable->setItem(loc, 1, bc);

        QTableWidgetItem *cont = new QTableWidgetItem();
        switch((*i).conflictType)
        {
        case ScheduleConflictTypes::startTimeOverlap:
            cont->setText("Item Unavailable");
            break;
        case ScheduleConflictTypes::endTimeOverlap:
            cont->setText("Item due back early");
            break;
        }
        cont->setFlags(cont->flags() ^ Qt::ItemIsEditable);
        ui->reportTable->setItem(loc, 2, cont);
        ui->reportTable->setRowHeight(loc, 80);

        // Map the index of the table to the conflict for easy plucking
        tableConflictMap.insert(loc, (*i));

        loc++;
    }
}

void FinalizeReservation::on_reportTable_clicked(const QModelIndex &index)
{

}

void FinalizeReservation::on_reportTable_doubleClicked(const QModelIndex &index)
{

}

void FinalizeReservation::on_resolveError_clicked()
{
    /*

        CREATE A MECHANISM TO CLOSE THIS WINDOW, TRIGGER CALLER TO RE-CALL EPHIMERAL FINALIZER.
        WE WANT TO RE-CALL FROM PARENT TO ENSURE THAT THE EXISITNG SIGNALS BOUND TO EPHIMERAL ARE
        HANDLED CORRECTLY. IDEALLY, IF ANY MORE ERRORS EXIST ONCE THIS WINDOW IS CLOSED, THE PARENT
        WILL RE-OPEN IT PRESENTING THE NEW OR STILL-EXISITNG ERRORS


        MAKE IT SO THAT WHEN AN ITEM IS CLICKED, OR MAYBE EVEN ON THE MAIN TABLE, A TIME OF
        'NEXT AVAIAILABLE' AND FOR HOW LONG IS LISTED.

    */
}

void FinalizeReservation::on_completeButton_clicked()
{

}

void FinalizeReservation::resizeEvent(QResizeEvent *event)
{

}

void FinalizeReservation::closeEvent(QCloseEvent *event)
{
    if(forceCancel)
    {
        emit forceClosed();
        event->accept();
    }

    if(unresolvedErrors == 0)
    {
        event->accept();
    } else {
        ui->errorLabel->setText("Unresolved errors are present. Can not complete.");
    }
}
