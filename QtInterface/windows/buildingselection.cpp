#include "buildingselection.h"
#include "ui_buildingselection.h"

BuildingSelection::BuildingSelection(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BuildingSelection)
{
    ui->setupUi(this);

    ui->toolBox->setCurrentIndex(0);
    onMain = true;
    buildstruct.names << "Administration" << "Consadine" << "Center for\nApplied\nSciences" << "Joshs\nBedroom" << "Moot" << "Toot" << "Foot"
              << "Loot" << "Qoot" << "Deer" << "Kitties" << "Fruit" << "Military\nIndustrial\nComplex";

    // JUST FOR TESTING UNTIL THE DATA BASE IS POPULATED
    foreach(QString b, buildstruct.names)
    {
        QStringList rooms;
        for(int i = 100; i < 250; i+=2)
        {
            rooms << QString::number(i);
        }
        buildstruct.nameToRoomsMap.insert(b, rooms);
    }

    buildButtonsGrid();
}

BuildingSelection::~BuildingSelection()
{
    delete ui;
}

void BuildingSelection::forceClose()
{
    this->close();
}

void BuildingSelection::buttonWasClicked()
{
    QPushButton *button = (QPushButton*)sender();
    qDebug() << "Button: " << button->text() << " was clicked!";
    selectedBuilding = button->text();
    buildRoomButtonGrid(button->text());
    onMain = false;
    ui->toolBox->setCurrentIndex(1);
}

void BuildingSelection::roomWasSelected()
{
    QPushButton *button = (QPushButton*)sender();
    qDebug() << "Room Button: " << button->text() << " was clicked!";
    selectedRoom = button->text();
    emit buildingAndRoomSelected(selectedBuilding, selectedRoom);
    this->close();
}

void BuildingSelection::buildButtonsGrid()
{
    int col = 0, row = col;
    foreach(QString building, buildstruct.names)
    {
        QPushButton * bldg = new QPushButton();
        bldg->setText(building);
        bldg->setMinimumHeight(100);
        bldg->setMinimumWidth(100);
        bldg->setMaximumHeight(200);
        bldg->setMaximumWidth(200);
        connect(bldg, SIGNAL(pressed()), this, SLOT(buttonWasClicked()));

        if(0 == col % 5)
        {
            row++;
            col = 0;
        }
        ui->buildingGrid->addWidget(bldg, row, col++);
    }
}

void BuildingSelection::buildRoomButtonGrid(QString bldgn)
{

    int col = 0, row = col;
    foreach(QString bulding, buildstruct.nameToRoomsMap.value(bldgn))
    {
        QPushButton * bldg = new QPushButton();
        bldg->setText(bulding);
        bldg->setMinimumHeight(100);
        bldg->setMinimumWidth(100);
        bldg->setMaximumHeight(200);
        bldg->setMaximumWidth(200);
        connect(bldg, SIGNAL(pressed()), this, SLOT(roomWasSelected()));

        if(0 == col % 5)
        {
            row++;
            col = 0;
        }
        ui->roomGrid->addWidget(bldg, row, col++);
    }

}

void BuildingSelection::on_toolBox_currentChanged(int index)
{
    if(onMain && index != 0)
        ui->toolBox->setCurrentIndex(0);
    if(!onMain && index != 1)
        ui->toolBox->setCurrentIndex(1);
}

void BuildingSelection::on_returnToBuildingWindow_clicked()
{
    onMain = true;
    ui->toolBox->setCurrentIndex(0);
}
