#include "displaynetworkerror.h"
#include "ui_displaynetworkerror.h"

DisplayNetworkError::DisplayNetworkError(ContextManager *context, DAMStatus status, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisplayNetworkError)
{
    ui->setupUi(this);

    localContext = context;
    setupWindow(status);
}

DisplayNetworkError::~DisplayNetworkError()
{
    delete ui;
}

void DisplayNetworkError::setupWindow(DAMStatus status)
{
    ui->errorOutput->insertPlainText("Preface:\n");
    ui->errorOutput->insertPlainText("\tThese errors are gone-through sequentially, meaning that while some may\n");
    ui->errorOutput->insertPlainText("\tshow up before others, their showing here first is not indicative of a\n");
    ui->errorOutput->insertPlainText("\tcascade failure. If the information in different errors seem to contradict\n");
    ui->errorOutput->insertPlainText("\teach-other it is recommended that you stop worrying, and RTFM.\n\n\n");
    ui->errorOutput->insertPlainText("Report:\n\n");
    ui->errorOutput->insertPlainText("Network Initialization:\n");
    ui->errorOutput->insertPlainText("\tLaunch Time:\t\t" + status.time_init.toString() + "\n");
    ui->errorOutput->insertPlainText("\tLast automated update:\t" + status.time_last_update.toString() + "\n\n");

    if(status.time_init == status.time_last_update)
    {
        ui->errorOutput->insertPlainText("IDENTIFIED:\n\tIt appears that the launch time of the network manager is equivalent to\n");
        ui->errorOutput->insertPlainText("\tthe time of the last automated update. This could indicate that the server is\n");
        ui->errorOutput->insertPlainText("\tcompletely unreachable. Please check the computer's network connection.\n");
        ui->errorOutput->insertPlainText("\tand press the 'retry' button below.\n");
        ui->errorOutput->insertPlainText("\tAll errors following this may have been caused by this issue.\n\n");
    }

    bool routine_error = false;
    int number_of_errors = 1;

    if(status.error_item.errorCode != 0)
    {
        routine_error = true;
        ui->errorOutput->insertPlainText("Error Number: " + QString::number(number_of_errors++));
        ui->errorOutput->insertPlainText("\n\nItem update error detected:\n\n");
        ui->errorOutput->insertPlainText("\tError Code: "+QString::number(status.error_item.errorCode));
        ui->errorOutput->insertPlainText("\n\tMessage: "+status.error_item.message);
        ui->errorOutput->insertPlainText("\n\tDescription: "+status.error_item.description);
    }

    if(status.error_res.errorCode != 0)
    {
        routine_error = true;
        ui->errorOutput->insertPlainText("\n\nError Number: " + QString::number(number_of_errors++));
        ui->errorOutput->insertPlainText("\n\nReservation update error detected:\n\n");
        ui->errorOutput->insertPlainText("\tError Code: "+QString::number(status.error_res.errorCode));
        ui->errorOutput->insertPlainText("\n\tMessage: "+status.error_res.message);
        ui->errorOutput->insertPlainText("\n\tDescription: "+status.error_res.description);
    }

    if(status.error_rem.errorCode != 0)
    {
        routine_error = true;
        ui->errorOutput->insertPlainText("\n\nError Number: " + QString::number(number_of_errors++));
        ui->errorOutput->insertPlainText("\n\nReminder update error detected:\n\n");
        ui->errorOutput->insertPlainText("\tError Code: "+QString::number(status.error_rem.errorCode));
        ui->errorOutput->insertPlainText("\n\tMessage: "+status.error_rem.message);
        ui->errorOutput->insertPlainText("\n\tDescription: "+status.error_rem.description);
    }

    if(status.error_cats.errorCode != 0)
    {
        routine_error = true;
        ui->errorOutput->insertPlainText("\n\nError Number: " + QString::number(number_of_errors++));
        ui->errorOutput->insertPlainText("\n\nReminder update error detected:\n\n");
        ui->errorOutput->insertPlainText("\tError Code: "+QString::number(status.error_cats.errorCode));
        ui->errorOutput->insertPlainText("\n\tMessage: "+status.error_cats.message);
        ui->errorOutput->insertPlainText("\n\tDescription: "+status.error_cats.description);
    }

    if (routine_error)
    {
        ui->errorOutput->insertPlainText("\n\nThe errors [1-" + QString::number(number_of_errors-1) + "] are triggered by");
        ui->errorOutput->insertPlainText(" routine operations that are performed automatically.");
        ui->errorOutput->insertPlainText("\tTypically, the messages displayed come directly from the EquipIT server.\n");
        ui->errorOutput->insertPlainText("\tIf you do not understand the errors, please see the program manual.\n");
    }

    if(status.error_other.errorCode != 0)
    {
        ui->errorOutput->insertPlainText("\nError Number: " + QString::number(number_of_errors++));
        ui->errorOutput->insertPlainText("\n\nNon-Routine error detected:\n\n");
        ui->errorOutput->insertPlainText("\tError Code: "+QString::number(status.error_other.errorCode));
        ui->errorOutput->insertPlainText("\n\tMessage: "+status.error_other.message);
        ui->errorOutput->insertPlainText("\n\tDescription: "+status.error_other.description);
        ui->errorOutput->insertPlainText("\n\n  This occurs when an error is thrown from an operation external\n");
        ui->errorOutput->insertPlainText("to automated recurring operations such-as pulling daily reservations,\n");
        ui->errorOutput->insertPlainText("items and reminders from the server. If you see this message, it is not\n");
        ui->errorOutput->insertPlainText("because the software is 'broken' it is because contact to the server\n");
        ui->errorOutput->insertPlainText("[" + QString(WEB_SERVER_ADDRESS) + "] is being blocked, or interrupted.\n\n");
    }
}

void DisplayNetworkError::on_pushButton_clicked()
{
    emit retrying();
    localContext->replayRequest();
    this->close();
}

void DisplayNetworkError::on_pushButton_2_clicked()
{
    emit ignoreIssue();
    this->close();
}
