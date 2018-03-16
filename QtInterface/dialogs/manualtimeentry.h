#ifndef MANUALTIMEENTRY_H
#define MANUALTIMEENTRY_H

#include <QDialog>
#include <QDebug>
#include <QString>
#include <QStringList>

namespace Ui {
class ManualTimeEntry;
}

class ManualTimeEntry : public QDialog
{
    Q_OBJECT

public:
    explicit ManualTimeEntry(QWidget *parent = 0);
    ~ManualTimeEntry();

signals:
    manualEntryComplete(QString);

private slots:
    void on_button1_clicked();

    void on_button2_clicked();

    void on_button3_clicked();

    void on_button4_clicked();

    void on_button5_clicked();

    void on_button6_clicked();

    void on_button7_clicked();

    void on_button8_clicked();

    void on_button9_clicked();

    void on_button0_clicked();

    void on_buttonAM_clicked();

    void on_buttonPM_clicked();

    void on_buttonUndo_clicked();

    void on_buttonClear_clicked();

    void on_completeButton_clicked();

private:
    Ui::ManualTimeEntry *ui;

    QString AMPM;
    QStringList lbuff;
    void updateBuffer(QString item);
    void updateDisplay();
};

#endif // MANUALTIMEENTRY_H
