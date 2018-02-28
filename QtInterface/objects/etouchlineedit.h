#ifndef ETOUCHLINEEDIT_H
#define ETOUCHLINEEDIT_H

#include <QDebug>
#include <QObject>
#include <QtWidgets>
#include <QFocusEvent>

class ETouchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    ETouchLineEdit(QWidget *parent = 0): QLineEdit(parent){}

signals:
    void focusGained();
    void focusLoss();

protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

};

#endif // ETOUCHLINEEDIT_H
