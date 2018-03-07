#include "addbuilding.h"
#include "ui_addbuilding.h"

AddBuilding::AddBuilding(ContextManager *context,
                         buildings currentBldgs, addBuildingInfo iAddType,
                         QString buildingName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBuilding)
{
    ui->setupUi(this);
    addType = iAddType;
    localContext = context;
    currentBuildings = currentBldgs;
    if (addType == addBuildingInfo::room)
    {
        if(buildingName.length() > 8)
            ui->label->setText(buildingName.mid(0,8) + "...");
        else
            ui->label->setText(buildingName);
    }

    screenBoard = new ScreenBoard(this);
    connect(screenBoard, SIGNAL(keyPress(QString)), this, SLOT(keyboardDataRecv(QString)));
    screenBoard->setAttribute(Qt::WA_DeleteOnClose, true);

    /*
        Setup MDI area that contains on-screen keyboard
    */
    screenSub = ui->mdiArea->addSubWindow(screenBoard);
    screenSub->setWindowFlags(Qt::FramelessWindowHint);
    screenSub->showMaximized();
}

AddBuilding::~AddBuilding()
{
    ui->mdiArea->removeSubWindow(screenBoard);
    localContext = 0;
    delete screenSub;
    delete ui;
}

void AddBuilding::forceClose()
{
    emit cancelled();
    this->close();
}

void AddBuilding::keyboardDataRecv(QString data)
{
    if (data == QString(SCREENBOARD_BACKSPACE))
    {
        ui->buildingNameEntered->setText(
                    ui->buildingNameEntered->text().left(
                        (ui->buildingNameEntered->text().length()-1)));
    } else {
        ui->buildingNameEntered->insert(data);
    }
}

void AddBuilding::on_addItem_clicked()
{
    QStringList searchList;
    if(addType == addBuildingInfo::building)
        searchList = currentBuildings.names;
    else
        searchList = currentBuildings.nameToRoomsMap[ui->label->text()];

    foreach(QString current, searchList)
    {
        if(ui->buildingNameEntered->text() == current)
        {
            ui->errorLabel->setText(current + " already exists..");
            ui->buildingNameEntered->clear();
            ui->buildingNameEntered->setFocus();
            return;
        }
    }

    emit returnItem(ui->buildingNameEntered->text());
    localContext->addUserCrumb("Added " + ui->buildingNameEntered->text());
    this->close();
}

void AddBuilding::on_cancelItem_clicked()
{
    emit cancelled();
    this->close();
}
