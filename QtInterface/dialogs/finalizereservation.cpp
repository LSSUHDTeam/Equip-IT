#include "finalizereservation.h"
#include "ui_finalizereservation.h"

FinalizeReservation::FinalizeReservation(Ephimeral *currentRes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FinalizeReservation)
{
    ui->setupUi(this);
    verified = false;
    forceCancel = false;
    unresolvedErrors = -1;
    currentSelection = -1;
    ephimeral = currentRes;
    tableHeaders << "Item Name" << "Barcode" << "Conflict Type";
    ui->reportTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setupTable();
}

FinalizeReservation::~FinalizeReservation()
{
    ephimeral = 0;
    delete ui;
}

void FinalizeReservation::removeItemFromReservation(int conflictMapIndex)
{
    // Remove Item from reservation
    ephimeral->removeItemFromReservationByBarcode(
                tableConflictMap[conflictMapIndex].itembarcode
                );

    emit conflictFlipped();
    this->close();
}

void FinalizeReservation::changeEndTimeToEarliestRequiredEndTime(QDateTime newDateTime)
{
    ephimeral->updateReservationStartTime(newDateTime);

    emit conflictFlipped();
    this->close();
}

void FinalizeReservation::forceClose()
{
    forceCancel = true;
    this->close();
}

void FinalizeReservation::setupTable()
{
    tableConflictMap.clear();
    ui->reportTable->clearContents();
    ui->resolveError->setEnabled(false);
    ui->completeButton->setEnabled(false);
    ui->reportTable->setColumnCount(tableHeaders.length());
    ui->reportTable->setHorizontalHeaderLabels(tableHeaders);
    ui->reportTable->setColumnWidth(0, (this->width()/3) - ((ui->resolveError->width()+10)/tableHeaders.length()) );
    ui->reportTable->setColumnWidth(1,(this->width()/3) - ((ui->resolveError->width()+10)/tableHeaders.length()));
    ui->reportTable->setColumnWidth(2,(this->width()/3) - ((ui->resolveError->width()+10)/tableHeaders.length())- 45);
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


        QFont cellFont;
        QTableWidgetItem *cont = new QTableWidgetItem();
        switch((*i).conflictType)
        {
        case ScheduleConflictTypes::startTimeOverlap:
            cellFont.setBold(true);
            cont->setText("Item Unavailable");
            break;
        case ScheduleConflictTypes::endTimeOverlap:
            cellFont.setItalic(true);
            cont->setText("Item due back early");
            break;
        }
        cont->setFont(cellFont);
        cont->setFlags(cont->flags() ^ Qt::ItemIsEditable);
        ui->reportTable->setItem(loc, 2, cont);
        ui->reportTable->setRowHeight(loc, 80);

        // Map the index of the table to the conflict for easy plucking
        tableConflictMap.insert(loc, (*i));

        loc++;
    }

    qDebug() << "UNRESOLVED ERRORS: " << conflicts.size();
    unresolvedErrors = conflicts.size();
    if(0==conflicts.size())
        ui->completeButton->setEnabled(true);
}

void FinalizeReservation::on_reportTable_clicked(const QModelIndex &index)
{
    currentSelection = index.row();
    ui->reportTable->selectRow(index.row());
    ui->resolveError->setEnabled(true);
}

void FinalizeReservation::launchResolver()
{
    if (currentSelection == -1)
        return;

}

void FinalizeReservation::on_reportTable_doubleClicked(const QModelIndex &index)
{
    if(currentSelection != index.row())
        ui->reportTable->selectRow(index.row());
    currentSelection = index.row();
    ui->resolveError->setEnabled(true);
    launchResolver();
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

    qDebug() << "Opening conflict resolver";

    ErrorResolver *er = new ErrorResolver(currentSelection, tableConflictMap);
    connect(er, SIGNAL(changeEndTime(QDateTime)),
            this, SLOT(changeEndTimeToEarliestRequiredEndTime(QDateTime)));
    connect(er, SIGNAL(removeItem(int)),
            this, SLOT(removeItemFromReservation(int)));
    er->setAttribute(Qt::WA_DeleteOnClose, true);
    er->showMaximized();
}

void FinalizeReservation::on_completeButton_clicked()
{
    qDebug() << "COMPLETE!!!! PUSH THAT B TO THE SERV";
}

void FinalizeReservation::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    setupTable();
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

void FinalizeReservation::on_reportTable_itemSelectionChanged()
{
    if (currentSelection == -1)
        return;

    currentSelection = ui->reportTable->currentRow();
    ui->reportTable->selectRow(currentSelection);
}
