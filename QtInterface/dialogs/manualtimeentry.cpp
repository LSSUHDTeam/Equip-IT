#include "manualtimeentry.h"
#include "ui_manualtimeentry.h"

ManualTimeEntry::ManualTimeEntry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManualTimeEntry)
{
    ui->setupUi(this);

    AMPM = "AM";
}

ManualTimeEntry::~ManualTimeEntry()
{
    delete ui;
}

void ManualTimeEntry::updateDisplay()
{
    QString display = "";
    if(lbuff.length() != 4)
    {
        int diff = 4 - lbuff.length();

        for(int i = 0; i < diff; i++)
            display = display + " ";
    }
    foreach(QString item, lbuff)
    {
        display = display + item;
    }
    display.insert(2, ":");

    QString hour = display.mid(0, 2);
    QString minute = display.mid(3, 5);

    int ihour = hour.toInt();
    int iminute = minute.toInt();

    if(ihour >= 1 && ihour <= 12 && iminute >= 0 && iminute <= 59)
    {
        ui->errorLabel->setText("Time is valid");
        ui->completeButton->setEnabled(true);
    } else {
        ui->errorLabel->setText("Invalid time");
        ui->completeButton->setEnabled(false);
    }
    display = display + " " + AMPM;
    ui->timeLabel->setText(display);
}

void ManualTimeEntry::updateBuffer(QString item)
{
    if(item == "UNDO")
    {
        if (lbuff.length() > 0)
            lbuff.removeLast();
    }
    else if(item == "AM" || item == "PM")
    {
        AMPM = item;
    }
    else if(item == "CLEAR")
    {
        lbuff.clear();
    }
    else if(lbuff.length() < 4)
    {
        lbuff.append(item);
    }
    updateDisplay();
}


void ManualTimeEntry::on_completeButton_clicked()
{
    if(ui->completeButton->isEnabled())
    {
        QString check = ui->timeLabel->text();
        check.replace(" ", "");
        check.insert(check.length()-2, " ");

        emit manualEntryComplete(check);
    }
    this->close();
}

void ManualTimeEntry::on_button1_clicked()
{
    updateBuffer("1");
}

void ManualTimeEntry::on_button2_clicked()
{
    updateBuffer("2");
}

void ManualTimeEntry::on_button3_clicked()
{
    updateBuffer("3");
}

void ManualTimeEntry::on_button4_clicked()
{
    updateBuffer("4");
}

void ManualTimeEntry::on_button5_clicked()
{
    updateBuffer("5");
}

void ManualTimeEntry::on_button6_clicked()
{
    updateBuffer("6");
}

void ManualTimeEntry::on_button7_clicked()
{
    updateBuffer("7");
}

void ManualTimeEntry::on_button8_clicked()
{
    updateBuffer("8");
}

void ManualTimeEntry::on_button9_clicked()
{
    updateBuffer("9");
}

void ManualTimeEntry::on_button0_clicked()
{
    updateBuffer("0");
}

void ManualTimeEntry::on_buttonAM_clicked()
{
    updateBuffer("AM");
}

void ManualTimeEntry::on_buttonPM_clicked()
{
    updateBuffer("PM");
}

void ManualTimeEntry::on_buttonUndo_clicked()
{
    updateBuffer("UNDO");
}

void ManualTimeEntry::on_buttonClear_clicked()
{
    updateBuffer("CLEAR");
}


