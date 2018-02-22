#ifndef DATAACCESSMANAGER_H
#define DATAACCESSMANAGER_H

#include <vector>

#include <QUrl>
#include <QMap>
#include <QTime>
#include <QString>
#include <QObject>
#include <QStringList>

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "structures.h"

struct DAMError {
    int errorCode;
    QString message;
    QString description;
};

struct DAMStatus {
    bool init_item, init_res, init_rem;
    bool last_item, last_res, last_rem;
    QTime time_init, time_last_update;

    DAMError error_item, error_res, error_rem;
};

class DataAccessManager : public QObject
{
    Q_OBJECT
public:
    explicit DataAccessManager(QObject *parent = 0);
    ~DataAccessManager();

    void updateLocalInformation();

signals:

    // Tie this to something to report when the network goes wrong
    // so people dont think that the program is broken...
    void networkError(DAMStatus);

private slots:

    // For populating local structures
    void net2item(QNetworkReply *reply);
    void net2res(QNetworkReply *reply);
    void net2rem(QNetworkReply *reply);

private:

    DAMStatus status_report;

    QMap<QString, QUrl> requestMap;

    QUrl itemFullRequest, resTodayRequest, remTodayRequest;

    bool item_busy, res_busy, rem_busy;
    QNetworkAccessManager *itemManager;
    QNetworkAccessManager *resManager;
    QNetworkAccessManager *remManager;

    std::vector<reservableItems> existingItems;
    std::vector<reservations> todaysReservations;
    std::vector<reservedReminders> todaysReminders;
};

#endif // DATAACCESSMANAGER_H
