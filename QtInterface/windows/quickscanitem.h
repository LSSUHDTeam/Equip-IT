#ifndef QUICKSCANITEM_H
#define QUICKSCANITEM_H

#include <QDebug>
#include <QDialog>
#include <QCloseEvent>
#include "framework/contextmanger.h"

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
    void dataReady(QStringList barcodes);

public slots:
    void forceClose();

private slots:
    void on_lineEdit_returnPressed();

    void on_pushButton_clicked();

    void on_toolBox_currentChanged(int index);

    void on_addItemConfirmation_clicked();

    void on_cancelButton_clicked();

    void on_cancelButton_2_clicked();

private:
    Ui::QuickScanItem *ui;
    ContextManager *localContext;
    QSIState currentState;

    reservableItems itemFound;
    std::vector<reservableItems> currentItems;
    QStringList peripheralList;
    bool noPeriphs;

    void buildConfirmationWindow();
};

#endif // QUICKSCANITEM_H
