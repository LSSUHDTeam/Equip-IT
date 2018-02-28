#include "screenboard.h"
#include "ui_screenboard.h"

ScreenBoard::ScreenBoard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ScreenBoard)
{
    ui->setupUi(this);
    shiftPresent = false;
}

ScreenBoard::~ScreenBoard()
{
    delete ui;
}

void ScreenBoard::forceClose()
{
    emit windowClosing();
}

void ScreenBoard::closeEvent(QCloseEvent *event)
{
    forceClose();
    event->ignore();
}

/*
    Keyboard Operations
*/

void ScreenBoard::flipKeysRegular()
{
    qDebug() << "flipRegulaaar!";
    QList<QPushButton*> list = this->findChildren<QPushButton*>();
    foreach(auto button, list)
    {
        QString newc;
        QString c = button->text();
        if("!" == c)
            newc = "1";
        else if("@" == c)
            newc = "2";
        else if("#" == c)
            newc = "3";
        else if("$" == c)
            newc = "4";
        else if("%" == c)
            newc = "5";
        else if("^" == c)
            newc = "6";
        else if("&&" == c)
            newc = "7";
        else if("*" == c)
            newc = "8";
        else if("(" == c)
            newc = "9";
        else if(")" == c)
            newc = "0";
        else {
            if (!c.contains(QString(".com")) && !c.contains(QString(".edu"))  &&
                    !c.contains(QString("SPACE")) &&
                    !c.contains(QString("FLIP")))
                newc = c.toLower();
            else
                newc = c;
        }
        button->setText(newc);
    }
}

void ScreenBoard::flipKeysShifted()
{
    qDebug() << "flipShufted!";
    QList<QPushButton*> list = this->findChildren<QPushButton*>();
    foreach(auto button, list)
    {
        QString newc;
        QString c = button->text();
        if("1" == c)
            newc = "!";
        else if("2" == c)
            newc = "@";
        else if("3" == c)
            newc = "#";
        else if("4" == c)
            newc = "$";
        else if("5" == c)
            newc = "%";
        else if("6" == c)
            newc = "^";
        else if("7" == c)
            newc = "&&";
        else if("8" == c)
            newc = "*";
        else if("9" == c)
            newc = "(";
        else if("0" == c)
            newc = ")";
        else {
            if (!c.contains(QString(".com")) && !c.contains(QString(".edu")) &&
                    !c.contains(QString("SPACE")) &&
                    !c.contains(QString("FLIP")))
                newc = c.toUpper();
            else
                newc = c;
        }
        button->setText(newc);
    }
}

void ScreenBoard::on_shutfButton_clicked()
{
    qDebug() << "SHUFT!";
    if (shiftPresent){
        flipKeysRegular();
        shiftPresent = false;
    } else {
        flipKeysShifted();
        shiftPresent = true;
    }
}

void ScreenBoard::on_spaceButton_clicked()
{
    emit keyPress(" ");
}

void ScreenBoard::on_yahooButton_clicked()
{
    emit keyPress("@yahoo.com");
}

void ScreenBoard::on_hotmailButton_clicked()
{
    emit keyPress("@hotmail.com");
}

void ScreenBoard::on_gmailButton_clicked()
{
    emit keyPress("@gmail.com");
}

void ScreenBoard::on_lssuButton_clicked()
{
    emit keyPress("@lssu.edu");
}

void ScreenBoard::on_button1_clicked()
{
    emit keyPress(ui->button1->text());
}

void ScreenBoard::on_button2_clicked()
{
    emit keyPress(ui->button2->text());
}

void ScreenBoard::on_button3_clicked()
{
    emit keyPress(ui->button3->text());
}

void ScreenBoard::on_button4_clicked()
{
    emit keyPress(ui->button4->text());
}

void ScreenBoard::on_button5_clicked()
{
    emit keyPress(ui->button5->text());
}

void ScreenBoard::on_button6_clicked()
{
    emit keyPress(ui->button6->text());
}

void ScreenBoard::on_button7_clicked()
{
    if(ui->button7->text() == "&&")
        emit keyPress("&");
    else
        emit keyPress(ui->button7->text());
}

void ScreenBoard::on_button8_clicked()
{
    emit keyPress(ui->button8->text());
}

void ScreenBoard::on_button9_clicked()
{
    emit keyPress(ui->button9->text());
}

void ScreenBoard::on_button0_clicked()
{
    emit keyPress(ui->button0->text());
}

void ScreenBoard::on_qButton_clicked()
{
    emit keyPress(ui->qButton->text());
}

void ScreenBoard::on_wButon_clicked()
{
    emit keyPress(ui->wButon->text());
}

void ScreenBoard::on_eButton_clicked()
{
    emit keyPress(ui->eButton->text());
}

void ScreenBoard::on_rButotn_clicked()
{
    emit keyPress(ui->rButotn->text());
}

void ScreenBoard::on_tButton_clicked()
{
    emit keyPress(ui->tButton->text());
}

void ScreenBoard::on_yButton_clicked()
{
    emit keyPress(ui->yButton->text());
}

void ScreenBoard::on_uButton_clicked()
{
    emit keyPress(ui->uButton->text());
}

void ScreenBoard::on_iButton_clicked()
{
    emit keyPress(ui->iButton->text());
}

void ScreenBoard::on_oButton_clicked()
{
    emit keyPress(ui->oButton->text());
}

void ScreenBoard::on_pButton_clicked()
{
    emit keyPress(ui->pButton->text());
}

void ScreenBoard::on_aButton_clicked()
{
    emit keyPress(ui->aButton->text());
}

void ScreenBoard::on_sButton_clicked()
{
    emit keyPress(ui->sButton->text());
}

void ScreenBoard::on_dButton_clicked()
{
    emit keyPress(ui->dButton->text());
}

void ScreenBoard::on_fButton_clicked()
{
    emit keyPress(ui->fButton->text());
}

void ScreenBoard::on_gButton_clicked()
{
    emit keyPress(ui->gButton->text());
}

void ScreenBoard::on_hButton_clicked()
{
    emit keyPress(ui->hButton->text());
}

void ScreenBoard::on_jButton_clicked()
{
    emit keyPress(ui->jButton->text());
}

void ScreenBoard::on_kButton_clicked()
{
    emit keyPress(ui->kButton->text());
}

void ScreenBoard::on_lButton_clicked()
{
    emit keyPress(ui->lButton->text());
}

void ScreenBoard::on_zButton_clicked()
{
    emit keyPress(ui->zButton->text());
}

void ScreenBoard::on_xButton_clicked()
{
    emit keyPress(ui->xButton->text());
}

void ScreenBoard::on_cButton_clicked()
{
    emit keyPress(ui->cButton->text());
}

void ScreenBoard::on_vButton_clicked()
{
    emit keyPress(ui->vButton->text());
}

void ScreenBoard::on_bButton_clicked()
{
    emit keyPress(ui->bButton->text());
}

void ScreenBoard::on_nButton_clicked()
{
    emit keyPress(ui->nButton->text());
}

void ScreenBoard::on_mButton_clicked()
{
    emit keyPress(ui->mButton->text());
}

void ScreenBoard::on_commaButton_clicked()
{
    emit keyPress(ui->commaButton->text());
}

void ScreenBoard::on_periodButton_clicked()
{
    emit keyPress(ui->periodButton->text());
}

void ScreenBoard::on_backspaceButton_clicked()
{
    emit keyPress(QString(SCREENBOARD_BACKSPACE));
}
