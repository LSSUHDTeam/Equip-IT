#ifndef DISPLAYNETWORKERROR_H
#define DISPLAYNETWORKERROR_H

#include <QDialog>
#include "framework/dataaccessmanager.h"
#include "framework/contextmanger.h"

namespace Ui {
class DisplayNetworkError;
}

class DisplayNetworkError : public QDialog
{
    Q_OBJECT

public:
    explicit DisplayNetworkError(ContextManager *context, DAMStatus status, QWidget *parent = 0);
    ~DisplayNetworkError();

signals:
    void ignoreIssue();
    void retrying();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::DisplayNetworkError *ui;

    ContextManager *localContext;
    void setupWindow(DAMStatus);

};

#endif // DISPLAYNETWORKERROR_H
