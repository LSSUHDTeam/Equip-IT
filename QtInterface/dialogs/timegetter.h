#ifndef TIMEGETTER_H
#define TIMEGETTER_H

#include <QDialog>

namespace Ui {
class TimeGetter;
}

class TimeGetter : public QDialog
{
    Q_OBJECT

public:
    explicit TimeGetter(QWidget *parent = 0);
    ~TimeGetter();

private:
    Ui::TimeGetter *ui;
};

#endif // TIMEGETTER_H
