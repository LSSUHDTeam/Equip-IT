#ifndef DISPLAYNETWORKERROR_H
#define DISPLAYNETWORKERROR_H

#include <QDialog>
#include "framework/dataaccessmanager.h"
#include "framework/contextmanger.h"

enum class NetworkCallerOrigin{
    primary,
    secondary
};

struct NetworkCallerConfig {
    bool allowRetry;
    NetworkCallerOrigin caller;
    DAMStatus status;

    NetworkCallerConfig(){}
    NetworkCallerConfig(NetworkCallerOrigin origin, DAMStatus stat, bool retry)
    {
        allowRetry = retry;
        caller = origin;
        status = stat;
    }
};

namespace Ui {
class DisplayNetworkError;
}

class DisplayNetworkError : public QDialog
{
    Q_OBJECT

public:
    explicit DisplayNetworkError(ContextManager *context, NetworkCallerConfig netConfig, QWidget *parent = 0);
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
    NetworkCallerConfig netConfiguration;
    void setupStandard();
    void setupCustom();

};

#endif // DISPLAYNETWORKERROR_H
