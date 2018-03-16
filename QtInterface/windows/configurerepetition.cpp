#include "configurerepetition.h"
#include "ui_configurerepetition.h"

ConfigureRepetition::ConfigureRepetition(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConfigureRepetition)
{
    ui->setupUi(this);
    repInterval = 1;
    selectedItem = "background-color: rgb(102, 102, 153); font: 12pt 'Terminal'; color: rgb(255, 255, 255)";
    regularItem = "background-color: rgb(102, 153, 153); font: 12pt 'Terminal'; color: rgb(255, 255, 255)";

    ui->dailySelect->setStyleSheet(selectedItem);
    ui->weeklyButton->setStyleSheet(regularItem);
    ui->monthSelect->setStyleSheet(regularItem);
    ui->yearlyButton->setStyleSheet(regularItem);

    ui->monday->setStyleSheet(regularItem);
    ui->tuesday->setStyleSheet(regularItem);
    ui->wednesday->setStyleSheet(regularItem);
    ui->thursday->setStyleSheet(regularItem);
    ui->friday->setStyleSheet(regularItem);

    currentRepetition.repType.clear();
    int today_dow = QDate::currentDate().day();

    switch(today_dow){
    case 1:
        ui->monday->setStyleSheet(selectedItem);
        currentRepetition.repType.push_back(repititionType::M);
        break;
    case 2:
        ui->tuesday->setStyleSheet(selectedItem);
        currentRepetition.repType.push_back(repititionType::T);
        break;
    case 3:
        ui->wednesday->setStyleSheet(selectedItem);
        currentRepetition.repType.push_back(repititionType::W);
        break;
    case 4:
        ui->thursday->setStyleSheet(selectedItem);
        currentRepetition.repType.push_back(repititionType::R);
        break;
    case 5:
        ui->friday->setStyleSheet(selectedItem);
        currentRepetition.repType.push_back(repititionType::F);
        break;
    default:
        ui->monday->setStyleSheet(selectedItem);
        currentRepetition.repType.push_back(repititionType::M);
        break;
    }
}

ConfigureRepetition::~ConfigureRepetition()
{
    delete ui;
}

void ConfigureRepetition::on_dailySelect_clicked()
{
    currentRepetition.repFreq = repititionFrequency::daily;
    updateButtons();
}

void ConfigureRepetition::on_monthSelect_clicked()
{
    currentRepetition.repFreq = repititionFrequency::monthly;
    updateButtons();
}

void ConfigureRepetition::on_yearlyButton_clicked()
{
    currentRepetition.repFreq = repititionFrequency::yearly;
    updateButtons();
}

void ConfigureRepetition::on_weeklyButton_clicked()
{
    currentRepetition.repFreq = repititionFrequency::weekly;
    updateButtons();
}

void ConfigureRepetition::on_addNumButton_clicked()
{
    repInterval += 1;
    ui->numberLabel->setText(QString::number(repInterval));
}

void ConfigureRepetition::on_subNumButton_clicked()
{
    if(repInterval == 1)
        return;

    repInterval -= 1;
    ui->numberLabel->setText(QString::number(repInterval));
}

void ConfigureRepetition::on_submitButton_clicked()
{
    emit repeatedComplete(currentRepetition);
    this->close();
}

void ConfigureRepetition::on_cancelButton_clicked()
{
    this->close();
}

void ConfigureRepetition::updateButtons()
{
    ui->dailySelect->setText(".");
    ui->monthSelect->setText(".");
    ui->weeklyButton->setText(".");
    ui->yearlyButton->setText(".");
    ui->dailySelect->setStyleSheet(regularItem);
    ui->weeklyButton->setStyleSheet(regularItem);
    ui->monthSelect->setStyleSheet(regularItem);
    ui->yearlyButton->setStyleSheet(regularItem);
    switch(currentRepetition.repFreq){
    case repititionFrequency::daily:
        ui->dailySelect->setText("X");
        ui->intervalLabel->setText("day(s)");
        ui->dailySelect->setStyleSheet(selectedItem);
        break;
    case repititionFrequency::weekly:
        ui->weeklyButton->setText("X");
        ui->intervalLabel->setText("week(s)");
        ui->weeklyButton->setStyleSheet(selectedItem);
        break;
    case repititionFrequency::monthly:
        ui->monthSelect->setText("X");
        ui->intervalLabel->setText("month(s)");
        ui->monthSelect->setStyleSheet(selectedItem);
        break;
    case repititionFrequency::yearly:
        ui->yearlyButton->setText("X");
        ui->intervalLabel->setText("year(s)");
        ui->yearlyButton->setStyleSheet(selectedItem);
        break;
    }
}

void ConfigureRepetition::on_monday_clicked()
{
    if (std::find(currentRepetition.repType.begin(),
                  currentRepetition.repType.end(),
                  repititionType::M) != currentRepetition.repType.end())
    {
        ui->monday->setStyleSheet(regularItem);
        currentRepetition.repType.erase(
                    std::remove(
                        currentRepetition.repType.begin(),
                        currentRepetition.repType.end(),
                        repititionType::M
                        ),
                    currentRepetition.repType.end());
    } else {
        ui->monday->setStyleSheet(selectedItem);
        currentRepetition.repType.push_back(repititionType::M);
    }
}

void ConfigureRepetition::on_tuesday_clicked()
{
    if (std::find(currentRepetition.repType.begin(),
                  currentRepetition.repType.end(),
                  repititionType::T) != currentRepetition.repType.end())
    {
        ui->tuesday->setStyleSheet(regularItem);
        currentRepetition.repType.erase(
                    std::remove(
                        currentRepetition.repType.begin(),
                        currentRepetition.repType.end(),
                        repititionType::T
                        ),
                    currentRepetition.repType.end());
    } else {
        ui->tuesday->setStyleSheet(selectedItem);
        currentRepetition.repType.push_back(repititionType::T);
    }
}

void ConfigureRepetition::on_wednesday_clicked()
{
    if (std::find(currentRepetition.repType.begin(),
                  currentRepetition.repType.end(),
                  repititionType::W) != currentRepetition.repType.end())
    {
        ui->wednesday->setStyleSheet(regularItem);
        currentRepetition.repType.erase(
                    std::remove(
                        currentRepetition.repType.begin(),
                        currentRepetition.repType.end(),
                        repititionType::W
                        ),
                    currentRepetition.repType.end());
    } else {
        ui->wednesday->setStyleSheet(selectedItem);
        currentRepetition.repType.push_back(repititionType::W);
    }
}

void ConfigureRepetition::on_thursday_clicked()
{
    if (std::find(currentRepetition.repType.begin(),
                  currentRepetition.repType.end(),
                  repititionType::R) != currentRepetition.repType.end())
    {
        ui->thursday->setStyleSheet(regularItem);
        currentRepetition.repType.erase(
                    std::remove(
                        currentRepetition.repType.begin(),
                        currentRepetition.repType.end(),
                        repititionType::R
                        ),
                    currentRepetition.repType.end());
    } else {
        ui->thursday->setStyleSheet(selectedItem);
        currentRepetition.repType.push_back(repititionType::R);
    }
}

void ConfigureRepetition::on_friday_clicked()
{
    if (std::find(currentRepetition.repType.begin(),
                  currentRepetition.repType.end(),
                  repititionType::F) != currentRepetition.repType.end())
    {
        ui->friday->setStyleSheet(regularItem);
        currentRepetition.repType.erase(
                    std::remove(
                        currentRepetition.repType.begin(),
                        currentRepetition.repType.end(),
                        repititionType::F
                        ),
                    currentRepetition.repType.end());
    } else {
        ui->friday->setStyleSheet(selectedItem);
        currentRepetition.repType.push_back(repititionType::F);
    }
}
