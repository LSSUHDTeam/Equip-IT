#ifndef CONFIGUREREPETITION_H
#define CONFIGUREREPETITION_H

#include <QMainWindow>
#include <QDate>
#include "framework/structures.h"
#include <algorithm>

namespace Ui {
class ConfigureRepetition;
}

class ConfigureRepetition : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConfigureRepetition(QWidget *parent = 0);
    ~ConfigureRepetition();

signals:
    repeatedComplete(repetition data);

private slots:
    void on_dailySelect_clicked();

    void on_monthSelect_clicked();

    void on_addNumButton_clicked();

    void on_subNumButton_clicked();

    void on_submitButton_clicked();

    void on_cancelButton_clicked();

    void on_yearlyButton_clicked();

    void on_weeklyButton_clicked();

    void on_monday_clicked();

    void on_tuesday_clicked();

    void on_wednesday_clicked();

    void on_thursday_clicked();

    void on_friday_clicked();

private:
    Ui::ConfigureRepetition *ui;

    repetition currentRepetition;
    QString selectedItem, regularItem;

    unsigned repInterval;
    void updateButtons();
};

#endif // CONFIGUREREPETITION_H
