#ifndef SCREENBOARD_H
#define SCREENBOARD_H

#include <QList>
#include <QDebug>
#include <QDesktopWidget>
#include <QPushButton>
#include <QMainWindow>
#include <QCloseEvent>

#define SCREENBOARD_BACKSPACE "<-"

namespace Ui {
class ScreenBoard;
}

class ScreenBoard : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScreenBoard(QWidget *parent = 0);
    ~ScreenBoard();

signals:
    void keyPress(QString key);
    void windowClosing();

public slots:
    void forceClose();

private slots:
    void on_shutfButton_clicked();

    void on_spaceButton_clicked();

    void on_yahooButton_clicked();

    void on_hotmailButton_clicked();

    void on_gmailButton_clicked();

    void on_lssuButton_clicked();

    void on_button1_clicked();

    void on_button2_clicked();

    void on_button3_clicked();

    void on_button5_clicked();

    void on_button4_clicked();

    void on_button6_clicked();

    void on_button7_clicked();

    void on_button8_clicked();

    void on_button9_clicked();

    void on_button0_clicked();

    void on_qButton_clicked();

    void on_wButon_clicked();

    void on_eButton_clicked();

    void on_rButotn_clicked();

    void on_tButton_clicked();

    void on_yButton_clicked();

    void on_uButton_clicked();

    void on_iButton_clicked();

    void on_oButton_clicked();

    void on_pButton_clicked();

    void on_aButton_clicked();

    void on_sButton_clicked();

    void on_dButton_clicked();

    void on_fButton_clicked();

    void on_gButton_clicked();

    void on_hButton_clicked();

    void on_jButton_clicked();

    void on_kButton_clicked();

    void on_lButton_clicked();

    void on_zButton_clicked();

    void on_xButton_clicked();

    void on_cButton_clicked();

    void on_vButton_clicked();

    void on_bButton_clicked();

    void on_nButton_clicked();

    void on_mButton_clicked();

    void on_commaButton_clicked();

    void on_periodButton_clicked();

    void on_backspaceButton_clicked();

private:
    Ui::ScreenBoard *ui;

    void flipKeysRegular();
    void flipKeysShifted();

    bool shiftPresent;

    void closeEvent(QCloseEvent *event);
};

#endif // SCREENBOARD_H
