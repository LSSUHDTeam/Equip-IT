#ifndef PERIPHERALVALIDATOR_H
#define PERIPHERALVALIDATOR_H

#include <QDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
#include "framework/structures.h"

struct PeriphScreenWrapper{
    peripherals periph;
    bool present;

    PeriphScreenWrapper(peripherals p, bool isPres){
        periph = p;
        present = isPres;
    }
};

namespace Ui {
class PeripheralValidator;
}

class PeripheralValidator : public QMainWindow
{
    Q_OBJECT

public:
    explicit PeripheralValidator(QWidget *parent = 0);
    ~PeripheralValidator();

signals:
    void periphsValidated(std::vector<peripherals> periphs);

public slots:
    void forceClose();
    void updateWindow(std::vector<peripherals> iperiphs);

private slots:
    void on_periphralTable_cellClicked(int row, int column);

private:
    Ui::PeripheralValidator *ui;

    QStringList peripheralList;
    std::vector<PeriphScreenWrapper> periphs;
    void setupTable();
    void buildLists();

    void resizeEvent(QResizeEvent *event);
};

#endif // PERIPHERALVALIDATOR_H
