#ifndef QUICKSCANITEM_H
#define QUICKSCANITEM_H

#include <QDebug>
#include <QDialog>
#include <QCloseEvent>
#include <QMdiSubWindow>
#include "windows/peripheralvalidator.h"
#include "framework/contextmanger.h"

#define MAX_FUTURE_BUFFER_MINUTES 60

enum class QSIState{
    ScanWindow,
    VerifyWindow
};

namespace Ui {
class QuickScanItem;
}

class QuickScanItem : public QDialog
{
    Q_OBJECT

public:
    explicit QuickScanItem(ContextManager *context, QWidget *parent = 0);
    ~QuickScanItem();

signals:
    void closeChildren();
    void dataReady(QStringList barcodes);
    void updatePeriphWindow(std::vector<peripherals>);
    void reservationComplete();

public slots:
    void forceClose();

    void updatePeriphs(std::vector<peripherals>);

private slots:
    void on_lineEdit_returnPressed();

    void on_pushButton_clicked();

    void on_toolBox_currentChanged(int index);

    void on_addItemConfirmation_clicked();

    void on_cancelButton_clicked();

    void on_cancelButton_2_clicked();

    void on_clearButton_clicked();

private:
    Ui::QuickScanItem *ui;
    ContextManager *localContext;
    QSIState currentState;

    reservableItems itemFound;
    std::vector<reservableItems> currentItems;
    bool noPeriphs;

    QMdiSubWindow *screenSub;
    PeripheralValidator *pvalidator;

    void buildConfirmationWindow();

};

#endif // QUICKSCANITEM_H
