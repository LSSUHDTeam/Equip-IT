#include "dataaccessmanager.h"

DataAccessManager::DataAccessManager(QObject *parent) : QObject(parent)
{
    // Setup DAM status card
    status_report.init_item = false;
    status_report.init_res = false;
    status_report.init_rem = false;
    status_report.last_item = false;
    status_report.last_res = false;
    status_report.last_rem = false;
    status_report.time_init = QTime::currentTime();
    status_report.time_last_update = QTime::currentTime();
    status_report.error_item.errorCode = 0;
    status_report.error_res.errorCode = 0;
    status_report.error_rem.errorCode = 0;

    // Internal-use blockers for forcing a wait on network calls
    item_busy = false; res_busy = false; rem_busy = false;

    // Network callers for categoris, and their connections
    itemManager = new QNetworkAccessManager();
    resManager = new QNetworkAccessManager();
    remManager = new QNetworkAccessManager();
    connect(itemManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(net2item(QNetworkReply*)));
    connect(resManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(net2res(QNetworkReply*)));
    connect(remManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(net2rem(QNetworkReply*)));

    // Request urls to the EquipIT API
    itemFullRequest = "http://h9k.lssu.edu/EquipIT/items.json";
    resTodayRequest = "";
    remTodayRequest = "";

    updateLocalInformation();
}

DataAccessManager::~DataAccessManager()
{
    delete itemManager;
    delete resManager;
    delete remManager;
}

void DataAccessManager::updateLocalInformation()
{
    if (!item_busy)
        itemManager->get(QNetworkRequest(itemFullRequest));

    /*

        NEED TO ADD THE OTHER 2 REQUESTS HERE!

    */


    status_report.time_last_update = QTime::currentTime();
}


void DataAccessManager::net2item(QNetworkReply *reply)
{
    item_busy = true;

    if (reply->error()){
        status_report.last_item = false;
        status_report.error_item.errorCode = -1;
        status_report.error_item.message = reply->errorString();
        status_report.error_item.description = "Failed running net2item";
        item_busy = false;
        return;
    }

    // BEGIN JSON STUFF

    QJsonDocument jsond = QJsonDocument::fromJson(QString(reply->readAll()).toUtf8());
    QJsonArray jsona = jsond.array();

    // Add each item in the inv list to existing items vector
    foreach (const QJsonValue &v, jsona)
    {
        reservableItems t_item;
        QJsonObject item = v.toObject();

        t_item.barcode = item["barcode"].toString();
        t_item.name = item["name"].toString();
        t_item.desc = item["desc"].toString();
      //  t_item.scheduleid = item["scheduleid"].toString();

        QJsonArray periphlist = item["periphs"].toArray();

        // Go through each peripheral and add it to the item's list
        foreach(const QJsonValue &p, periphlist)
        {
            peripherals t_periph;
            QJsonObject periph = p.toObject();
            t_periph.name = periph["name"].toString();
            t_periph.desc = periph["desc"].toString();
            t_periph.count = periph["count"].toInt();
            t_periph.numberpresent = periph["numberpresent"].toInt();

            t_item.periphs.push_back(t_periph);
        }

        // Add item to items list
        existingItems.push_back(t_item);
    }

    // END JSON STUFF - FLAG IT OUT
    if (!status_report.init_item)
        status_report.init_item = true;

    item_busy = false;
    status_report.last_item = true;
}

void DataAccessManager::net2res(QNetworkReply *reply)
{
    res_busy = true;

    if (reply->error()){
        status_report.last_res = false;
        status_report.error_res.errorCode = -1;
        status_report.error_res.message = reply->errorString();
        status_report.error_res.description = "Failed running net2item";
        res_busy = false;
        return;
    }


    /*

                DO JSON STUFF HERE

    */



    if (!status_report.init_res)
        status_report.init_res = true;

    res_busy = false;
    status_report.last_res = true;
}

void DataAccessManager::net2rem(QNetworkReply *reply)
{
    rem_busy = true;

    if (reply->error()){
        status_report.last_rem = false;
        status_report.error_rem.errorCode = -1;
        status_report.error_rem.message = reply->errorString();
        status_report.error_rem.description = "Failed running net2item";
        rem_busy = false;
        return;
    }


    /*

                DO JSON STUFF HERE

    */


    if (!status_report.init_rem)
        status_report.init_rem = true;

    rem_busy = false;
    status_report.last_rem = true;
}

