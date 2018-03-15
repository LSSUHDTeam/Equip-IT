#ifndef ITEMCONFIRMATIONDIALOG_H
#define ITEMCONFIRMATIONDIALOG_H

#include <QDebug>
#include <QMap>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMainWindow>
#include <QPushButton>
#include "framework/contextmanger.h"
#include "framework/structures.h"
#include "framework/ephimeral.h"
#include "windows/peripheralvalidator.h"


namespace Ui {
class ItemConfirmationDialog;
}

class ItemConfirmationDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit ItemConfirmationDialog(ContextManager * context,
                                    std::vector<reservableItems> selected,
                                    QWidget *parent = 0);
    ~ItemConfirmationDialog();

public slots:
    void forceClose();

    void periphsUpdatedByValidator(std::vector<peripherals>);

signals:
    void closeChildren();
    void confirmed(std::vector<reservableItems>);
    void pushCurrentItemToPeripheralValidator(std::vector<peripherals>);

private slots:
    void on_scrollUp_clicked();

    void on_scrollDown_clicked();

    void on_item_button_clicked();

    void on_complete_clicked();

private:
    Ui::ItemConfirmationDialog *ui;
    std::vector<reservableItems> selectedItems;
    ContextManager * localContext;
    QMdiSubWindow *periphWindow;
    PeripheralValidator *pvalidate;
    QMap<QString, reservableItems> buttonTextToItemMap;
    QString currentUserSelection;

    void buildItemButtons();

    void loadPeripherals();

    void shutdownWindow();
};

#endif // ITEMCONFIRMATIONDIALOG_H
