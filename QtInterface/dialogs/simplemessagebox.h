#ifndef SIMPLEMESSAGEBOX_H
#define SIMPLEMESSAGEBOX_H

#include <QDialog>
#include <QCloseEvent>

struct smbdata {
    QString from, title;
    QStringList messageData;

    smbdata(QString origin, QString header, QStringList message)
    {
        from = origin; title = header; messageData = message;
    }
};

namespace Ui {
class SimpleMessageBox;
}

class SimpleMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit SimpleMessageBox(smbdata messageData, QWidget *parent = 0);
    ~SimpleMessageBox();

signals:
    void messageBoxClosed();

public slots:
    void forceClose();

private slots:
    void on_okayButton_clicked();

private:
    bool forced;
    Ui::SimpleMessageBox *ui;
    void closeEvent(QCloseEvent *);
};

#endif // SIMPLEMESSAGEBOX_H
