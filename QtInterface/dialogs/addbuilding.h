#ifndef ADDBUILDING_H
#define ADDBUILDING_H

#include <QMdiArea>
#include <QMdiSubWindow>
#include <QDialog>
#include "framework/structures.h"
#include "windows/screenboard.h"
#include "framework/contextmanger.h"

enum class addBuildingInfo {
    building,
    room
};

namespace Ui {
class AddBuilding;
}

class AddBuilding : public QDialog
{
    Q_OBJECT

public:
    explicit AddBuilding(ContextManager *context, buildings currentBldgs,
                         addBuildingInfo addType,  QString buildingName = "",
                         QWidget *parent = 0);
    ~AddBuilding();
signals:
    void cancelled();
    void returnItem(QString newItem);

public slots:
    void forceClose();
    void keyboardDataRecv(QString);

private slots:
    void on_addItem_clicked();

    void on_cancelItem_clicked();

private:
    Ui::AddBuilding *ui;
    addBuildingInfo addType;
    ScreenBoard *screenBoard;
    QMdiSubWindow *screenSub;
    ContextManager *localContext;
    buildings currentBuildings;

};

#endif // ADDBUILDING_H
