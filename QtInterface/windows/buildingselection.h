#ifndef BUILDINGSELECTION_H
#define BUILDINGSELECTION_H

#include <QMap>
#include <QSize>
#include <QFile>
#include <QDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QTextStream>
#include <QStandardPaths>
#include <QByteArray>

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "dialogs/addbuilding.h"
#include "framework/structures.h"
#include "framework/contextmanger.h"

namespace Ui {
class BuildingSelection;
}

class BuildingSelection : public QMainWindow
{
    Q_OBJECT

public:
    explicit BuildingSelection(ContextManager *context, QWidget *parent = 0);
    ~BuildingSelection();
signals:
    void buildingAndRoomSelected(QString building, QString room);
    void closeChildren();

public slots:
    void forceClose();

    void addNewBuilding(QString);
    void addNewRoom(QString);
    void addActionCancelled();

private slots:
    void buttonWasClicked();

    void roomWasSelected();

    void on_toolBox_currentChanged(int index);

    void on_returnToBuildingWindow_clicked();

    void new_building();
    void new_room();
    void add_cancelled();

private:
    Ui::BuildingSelection *ui;

    bool onMain, allowAdds;
    QSize btnMin, btnMax;

    buildings buildstruct;
    ContextManager * localContext;

    int building_col, building_row, room_col, room_row;
    QString selectedBuilding, selectedRoom, fileLocation;

    void loadBuildings();

    void insertItem(QString item, bool isBuilding);

    void buildButtonsGrid();
    void buildRoomButtonGrid(QString building);

    void writeBuildings();


};

#endif // BUILDINGSELECTION_H
