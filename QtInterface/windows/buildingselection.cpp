#include "buildingselection.h"
#include "ui_buildingselection.h"

BuildingSelection::BuildingSelection(ContextManager *context, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BuildingSelection)
{
    ui->setupUi(this);

    localContext = context;
    fileLocation =  QStandardPaths::writableLocation(
                QStandardPaths::DataLocation) + "/resources/buildings.json";

    btnMin.setWidth(100); btnMin.setHeight(100);
    btnMax.setWidth(250); btnMax.setHeight(250);
    building_col = 0; building_row = 0; room_col = 0; room_row = 0;

    ui->toolBox->setCurrentIndex(0);
    onMain = true;
    allowAdds = true;

    loadBuildings();
    buildButtonsGrid();
}

BuildingSelection::~BuildingSelection()
{
    localContext = 0;
    delete ui;
}

void BuildingSelection::forceClose()
{
    emit closeChildren();
    this->close();
}

void BuildingSelection::addNewBuilding(QString name)
{
    QStringList rooms;
    buildstruct.names.append(name);
    buildstruct.nameToRoomsMap.insert(name, rooms);
    insertItem(name, true);
    writeBuildings();
    ui->statusbar->showMessage("Building Added");
    allowAdds = true;
}

void BuildingSelection::addNewRoom(QString name)
{
    buildstruct.nameToRoomsMap[selectedBuilding].append(name);
    insertItem(name, false);
    writeBuildings();
    ui->statusbar->showMessage("Room added");
    allowAdds = true;
}

void BuildingSelection::addActionCancelled()
{
    ui->statusbar->showMessage("Action cancelled");
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

    QPushButton * newbldg = new QPushButton();
    newbldg->setText("Add\nBuilding");
    newbldg->setMinimumSize(btnMin);
    newbldg->setMaximumSize(btnMax);
    connect(newbldg, SIGNAL(pressed()), this, SLOT(new_building()));
    ui->buildingGrid->addWidget(newbldg, building_row, building_col++);

    foreach(QString building, buildstruct.names)
    {
        insertItem(building, true);
    }
}

void BuildingSelection::buildRoomButtonGrid(QString bldgn)
{
    room_col = 0; room_row = 0;
    qDeleteAll(ui->roomGrid->children());

    QPushButton * newRoom = new QPushButton();
    newRoom->setText("Add\nRoom");
    newRoom->setMinimumSize(btnMin);
    newRoom->setMaximumSize(btnMax);
    connect(newRoom, SIGNAL(pressed()), this, SLOT(new_room()));
    ui->roomGrid->addWidget(newRoom, room_row, room_col++);

    foreach(QString room, buildstruct.nameToRoomsMap.value(bldgn))
    {
        insertItem(room, false);
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

void BuildingSelection::new_building()
{
    if(allowAdds)
    {
        allowAdds = false;
        AddBuilding *add = new AddBuilding(localContext, buildstruct, addBuildingInfo::building,"",this);
        connect(add, SIGNAL(returnItem(QString)), this, SLOT(addNewBuilding(QString)));
        connect(add, SIGNAL(cancelled()), this, SLOT(add_cancelled()));
        connect(this, SIGNAL(closeChildren()), add, SLOT(forceClose()));
        add->setAttribute(Qt::WA_DeleteOnClose, true);
        add->showMaximized();
    }
}

void BuildingSelection::new_room()
{
    if(allowAdds)
    {
        allowAdds = false;
        AddBuilding *add = new AddBuilding(localContext, buildstruct, addBuildingInfo::room, selectedBuilding,this);
        connect(add, SIGNAL(returnItem(QString)), this, SLOT(addNewRoom(QString)));
        connect(add, SIGNAL(cancelled()), this, SLOT(add_cancelled()));
        connect(this, SIGNAL(closeChildren()), add, SLOT(forceClose()));
        add->setAttribute(Qt::WA_DeleteOnClose, true);
        add->showMaximized();
    }
}

void BuildingSelection::add_cancelled()
{
    allowAdds = true;
}

void BuildingSelection::loadBuildings()
{
    QFile jsonFile(fileLocation);
    jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString jsonData = jsonFile.readAll();
    jsonFile.close();
    QJsonDocument buildingData = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonArray jsona = buildingData.array();

    foreach(const QJsonValue &v, jsona)
    {
        QJsonObject bld = v.toObject();
        buildstruct.names.append(bld["name"].toString());
        QJsonArray rooms = bld["rooms"].toArray();

        QStringList roomList;
        foreach(const QJsonValue &i, rooms)
        {
            roomList.append(i.toString());
        }
        buildstruct.nameToRoomsMap.insert(bld["name"].toString(), roomList);
    }
}

void BuildingSelection::writeBuildings()
{
    QString output = "[";
    foreach (const QString &name, buildstruct.names)
    {
        output.append("\n\t{\"name\": \""+name+"\",\n\t\"rooms\": [");
        foreach(const QString &room, buildstruct.nameToRoomsMap[name])
        {
            output.append("\n\t\t\"" + room + "\",");
        }
        output = output.left(output.length()-1);
        output.append("\n\t]},");
    }
    output = output.left(output.length()-1);
    output.append("\n]");

    QFile writeOut(fileLocation);
    writeOut.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QTextStream outStream(&writeOut);
    outStream << output;
    writeOut.close();
}

void BuildingSelection::insertItem(QString item, bool isBuilding)
{
    if(isBuilding)
    {
        QPushButton * bldg = new QPushButton();
        bldg->setText(item);
        bldg->setMinimumSize(btnMin);
        bldg->setMaximumSize(btnMax);
        connect(bldg, SIGNAL(pressed()), this, SLOT(buttonWasClicked()));

        if(0 == building_col % 5)
        {
            building_row++;
            building_col = 0;
        }
        ui->buildingGrid->addWidget(bldg, building_row, building_col++);
    }
    else
    {
        QPushButton * room = new QPushButton();
        room->setText(item);
        room->setMinimumSize(btnMin);
        room->setMaximumSize(btnMax);
        connect(room, SIGNAL(pressed()), this, SLOT(roomWasSelected()));

        if(0 == room_col % 5)
        {
            room_row++;
            room_col = 0;
        }
        ui->roomGrid->addWidget(room, room_row, room_col++);
    }
}
