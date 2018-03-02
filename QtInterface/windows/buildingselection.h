#ifndef BUILDINGSELECTION_H
#define BUILDINGSELECTION_H

#include <QMainWindow>
#include <QDebug>
#include <QPushButton>
#include <QMap>

#include "framework/structures.h"

namespace Ui {
class BuildingSelection;
}

class BuildingSelection : public QMainWindow
{
    Q_OBJECT

public:
    explicit BuildingSelection(QWidget *parent = 0);
    ~BuildingSelection();
signals:
    void buildingAndRoomSelected(QString building, QString room);

public slots:
    void forceClose();

private slots:
    void buttonWasClicked();

    void roomWasSelected();

    void on_toolBox_currentChanged(int index);

    void on_returnToBuildingWindow_clicked();

private:
    Ui::BuildingSelection *ui;

    bool onMain;

    buildings buildstruct;

    QString selectedBuilding, selectedRoom;

    void buildButtonsGrid();
    void buildRoomButtonGrid(QString building);
};

#endif // BUILDINGSELECTION_H
