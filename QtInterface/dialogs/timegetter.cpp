#include "timegetter.h"
#include "ui_timegetter.h"

TimeGetter::TimeGetter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeGetter)
{
    ui->setupUi(this);
}

TimeGetter::~TimeGetter()
{
    delete ui;
}
