#ifndef CATITEMS_H
#define CATITEMS_H

#include <algorithm>
#include <QDebug>
#include <QMap>
#include <QSize>
#include <QPushButton>
#include <QMainWindow>
#include <QCloseEvent>
#include <QStringList>
#include "dialogs/iteminfodisplay.h"
#include "dialogs/itemconfirmationdialog.h"
#include "framework/ephimeral.h"
#include "framework/contextmanger.h"
#include "framework/structures.h"
#include "framework/ephimeral.h"

struct catWrap{
    itemCategories catDefinition;
    std::vector<itemCategories> subCats;
    std::vector<reservableItems> itemsInCat;

    bool operator==(const QString &label)
    {
        return (catDefinition.label == label);
    }
};

namespace Ui {
class CatItems;
}

class CatItems : public QMainWindow
{
    Q_OBJECT

public:
    explicit CatItems(ContextManager *context, Ephimeral *ephim, QWidget *parent = 0);
    ~CatItems();

signals:
    void closeChildren();
    void catItemsClosed();
    void dataReady(QStringList);

public slots:
    void forceClose();

private slots:
    void on_searchButton_clicked();

    void on_finalizeButton_clicked();

    void on_upButton_clicked();

    void on_backButton_clicked();

    void on_downButton_clicked();

    void some_cat_click();

    void some_item_click();

    void some_item_unclick();

    void some_unavailable_click();

    void on_homeButton_clicked();

    void itemsConfirmedByUser(std::vector<reservableItems> confirmedItems);

private:
    Ui::CatItems *ui;

    ContextManager *localContext;

    int row, col;
    std::vector<reservableItems> nonselectedItems;
    std::vector<reservableItems> itemsSelected;
    QMap<QString, catWrap> buttonMap;
    QMap<QString, QString> labelMap;
    QStringList catPath;
    QSize minb, maxb;
    QString selectedButton, standardButton, catButtonmeow, unavailButton;
    Ephimeral *curentReservation;
    timespecificItems timeCache;

    void loadButtonMap();
    void updateGrid();

    void enterCatPath(QString path);
    void popCatPath();
    void updateCatPath();

    void shutdownWindow();
    void closeEvent(QCloseEvent *event);
};

#endif // CATITEMS_H
