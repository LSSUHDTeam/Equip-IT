#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QMainWindow>

#include <QFile>
#include <QString>
#include <QByteArray>
#include <QCloseEvent>
#include <QStandardPaths>
#include <QCryptographicHash>
#include "framework/contextmanger.h"

namespace Ui {
class AdminPanel;
}

class AdminPanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminPanel(ContextManager * context, QWidget *parent = 0);
    ~AdminPanel();

signals:
    void adminPanelClosed(WindowDescriptors);

public slots:
    void requestForceClose();

private slots:
    void on_b1_clicked();

    void on_b2_clicked();

    void on_b3_clicked();

    void on_b4_clicked();

    void on_b5_clicked();

    void on_b6_clicked();

    void on_b7_clicked();

    void on_b8_clicked();

    void on_b9_clicked();

    void on_b0_clicked();

    void on_bA_clicked();

    void on_bB_clicked();

    void on_bC_clicked();

    void on_bD_clicked();

    void on_bE_clicked();

    void on_bE_2_clicked();

    void on_setPasswordButton_clicked();

    void on_clearButton_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::AdminPanel *ui;
    ContextManager *localContext;

    void closeEvent(QCloseEvent *event);
};

#endif // ADMINPANEL_H
