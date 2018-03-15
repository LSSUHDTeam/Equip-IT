#ifndef ITEMINFODISPLAY_H
#define ITEMINFODISPLAY_H

#include <QDialog>
#include <QDebug>
#include "framework/structures.h"

struct ItemInfoWrap {
    reservableItems item;
    schedule itemSchedule;
    bool available;
    ItemInfoWrap(){
        available = false;
    }
    ItemInfoWrap(reservableItems itemStruct,
                 schedule schedStruct, bool availability)
    {
        item = itemStruct; itemSchedule = schedStruct;
        available = availability;
    }
};

namespace Ui {
class ItemInfoDisplay;
}

class ItemInfoDisplay : public QDialog
{
    Q_OBJECT

public:
    explicit ItemInfoDisplay(ItemInfoWrap item, QWidget *parent = 0);
    ~ItemInfoDisplay();

private slots:
    void on_closeButton_clicked();

private:
    Ui::ItemInfoDisplay *ui;

    ItemInfoWrap itemInfo;

    void buildDisplay();
};

#endif // ITEMINFODISPLAY_H
