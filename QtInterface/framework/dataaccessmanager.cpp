#include "dataaccessmanager.h"

DataAccessManager::DataAccessManager(QObject *parent) : QObject(parent)
{
    // Ensure we can keep track of full update cycles
    localTicker = 0;
    connect(this, SIGNAL(localCheckIn()), this, SLOT(localNetCheck()));

    /*
    // Setup DAM status card
    status_report.init_item = true;
    status_report.init_res = true;
    status_report.init_rem = true;
    status_report.last_item = true;
    status_report.last_res = true;
    status_report.last_rem = true;
    status_report.time_init = QTime::currentTime();
    status_report.time_last_update = QTime::currentTime();
    status_report.error_item.errorCode = 0;
    status_report.error_res.errorCode = 0;
    status_report.error_rem.errorCode = 0;
    status_report.error_other.errorCode = 0;
*/

    // Internal-use blockers for forcing a wait on network calls
    item_busy = false; res_busy = false; rem_busy = false; cats_busy = false;

    // Network callers for categories, and their connections
    itemManager = new QNetworkAccessManager();
    resManager = new QNetworkAccessManager();
    remManager = new QNetworkAccessManager();
    catManager = new QNetworkAccessManager();
    alienManager = new QNetworkAccessManager();

    connect(itemManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateExistingItems(QNetworkReply*)));
    connect(resManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateTodaysReservations(QNetworkReply*)));
    connect(remManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateTodaysReminders(QNetworkReply*)));
    connect(catManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateCategories(QNetworkReply*)));
    connect(alienManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(alienResponse(QNetworkReply*)));

    // API Token - This will be put to file eventually, and will change once server goes live
    apiToken = "fefd8a1a97021dbab2d105c4784a1906cd89fc575009387d378b8807192c16e3";

    // QUrl has the ability to '.addQuery' and build urls, but the ones we use are few,
    // and won't change
    requestMap["getAllItems"] = QUrl(QString(WEB_SERVER_ADDRESS)+"/items?classifier=all&token=" + apiToken);
    requestMap["getSchedules"] = QUrl(QString(WEB_SERVER_ADDRESS)+"/schedules?classifier=all&token=" + apiToken);
    requestMap["getTodaysRes"] = QUrl(QString(WEB_SERVER_ADDRESS)+"/reservations?frame=today&token=" + apiToken);
    requestMap["getAllRes"] = QUrl(QString(WEB_SERVER_ADDRESS)+"/reservations?frame=all&token=" + apiToken);
    requestMap["getTodaysRem"] = QUrl(QString(WEB_SERVER_ADDRESS)+"/reminders?frame=today&token=" + apiToken);
    requestMap["getAllRem"] = QUrl(QString(WEB_SERVER_ADDRESS)+"/reminders?frame=all&token=" + apiToken);
    requestMap["getAllCats"] = QUrl(QString(WEB_SERVER_ADDRESS)+"/cats?token=" + apiToken);

    updateLocalInformation(WindowDescriptors::_non_window_DAM);
}

DataAccessManager::~DataAccessManager()
{
    delete itemManager;
    delete resManager;
    delete remManager;
    delete catManager;
}

void DataAccessManager::updateLocalInformation(WindowDescriptors caller)
{
    lastRequest.caller = caller;
    lastRequest.isFullUpdate = true;

    qDebug() << "Performing uli";
    while(item_busy || res_busy || rem_busy || cats_busy){}
    runningFullUpdate = true;

    itemManager->get(QNetworkRequest(requestMap["getAllItems"]));
    resManager->get(QNetworkRequest(requestMap["getTodaysRes"]));
    remManager->get(QNetworkRequest(requestMap["getTodaysRem"]));
    catManager->get(QNetworkRequest(requestMap["getAllCats"]));

    qDebug() << "All requests sent";
}

void DataAccessManager::replayLastRequest()
{
    qDebug() << "Retrying last query";

    // Full updates are special
    if (lastRequest.isFullUpdate)
    {
        qDebug() << "Last query was a full update";
        updateLocalInformation(lastRequest.caller);
        return;
    }

    alienDataType = lastRequest.format;
    if(lastRequest.isGet)
        alienManager->get(QNetworkRequest(lastRequest.request));
    else
    {
        qDebug() << "\n\n\n\n\t\tSOMETHING COMPLETLY UNTESTED IS ABOUT TO HAPPEN...............\n";
        alienManager->post(QNetworkRequest(lastRequest.request),
                           lastRequest.postQuery.toString(QUrl::FullyEncoded).toUtf8());
    }
}

void DataAccessManager::localNetCheck()
{
    qDebug() << "CHECK";

    if (runningFullUpdate)
    {
        localTicker++;
        if (localTicker == 4)
        {
            localTicker = 0;
            if( (!status_report.last_item) || (!status_report.last_rem) ||
                    (!status_report.last_res) || (!status_report.last_cats) )
            {
                emit networkError(status_report);
            }
            else
            {
                qDebug() << "COMPLETED - EMITTING";
                status_report.time_last_update = QTime::currentTime();
                emit dataUpdated();
            }
        }
    }
}


void DataAccessManager::updateExistingItems(QNetworkReply *reply)
{
    item_busy = true;

    if (reply->error()){
        status_report.last_item = false;
        status_report.error_item = DAMError(-1, reply->errorString(), "Failed to update existing items",
                                            DAMOrigin( WindowDescriptors::_non_window_DAM, requestMap["getAllItems"]));
        item_busy = false;
        emit localCheckIn();
        return;
    }

    QString qreply =  QString(reply->readAll());
    DAMError errCheck = scanForErrorMessage(qreply);
    if(errCheck.errorCode != 0)
    {
        status_report.last_item = false;
        status_report.error_item = errCheck;
        status_report.error_item.origin_of_request.caller = WindowDescriptors::_non_window_DAM;
        status_report.error_item.origin_of_request.request = requestMap["getAllItems"];
        item_busy = false;
        emit localCheckIn();
        return;
    }

    existingItems.clear();
    existingItems = jsonToItemData(qreply);

    if (!status_report.init_item)
        status_report.init_item = true;

    item_busy = false;
    status_report.last_item = true;
    emit localCheckIn();
}

void DataAccessManager::updateTodaysReservations(QNetworkReply *reply)
{
    res_busy = true;

    if (reply->error()){
        status_report.last_res = false;
        status_report.error_res = DAMError(-1, reply->errorString(), "Failed to get today's reservations",
                                            DAMOrigin( WindowDescriptors::_non_window_DAM, requestMap["getTodaysRes"]));
        res_busy = false;
        emit localCheckIn();
        return;
    }

    QString qreply =  QString(reply->readAll());
    DAMError errCheck = scanForErrorMessage(qreply);
    if(errCheck.errorCode != 0)
    {
        status_report.last_res = false;
        status_report.error_res = errCheck;
        status_report.error_res.origin_of_request.caller = WindowDescriptors::_non_window_DAM;
        status_report.error_res.origin_of_request.request = requestMap["getTodaysRes"];
        res_busy = false;
        emit localCheckIn();
        return;
    }

    todaysReservations.clear();
    todaysReservations = jsonToResData(qreply);

    if (!status_report.init_res)
        status_report.init_res = true;

    res_busy = false;
    status_report.last_res = true;
    emit localCheckIn();
}

void DataAccessManager::updateTodaysReminders(QNetworkReply *reply)
{
    rem_busy = true;

    if (reply->error()){
        status_report.last_rem = false;
        status_report.error_rem = DAMError(-1, reply->errorString(), "Failed to get today's reminders",
                                            DAMOrigin( WindowDescriptors::_non_window_DAM, requestMap["getTodaysRem"]));
        rem_busy = false;
        emit localCheckIn();
        return;
    }

    QString qreply =  QString(reply->readAll());
    DAMError errCheck = scanForErrorMessage(qreply);
    if(errCheck.errorCode != 0)
    {
        status_report.last_rem = false;
        status_report.error_rem = errCheck;
        status_report.error_rem.origin_of_request.caller = WindowDescriptors::_non_window_DAM;
        status_report.error_rem.origin_of_request.request = requestMap["getTodaysRem"];
        rem_busy = false;
        emit localCheckIn();
        return;
    }

    todaysReminders.clear();
    todaysReminders = jsonToRemData(qreply);

    if (!status_report.init_rem)
        status_report.init_rem = true;

    rem_busy = false;
    status_report.last_rem = true;
    emit localCheckIn();
}

void DataAccessManager::updateCategories(QNetworkReply *reply)
{
    cats_busy = true;

    if (reply->error()){
        status_report.last_cats = false;
        status_report.error_cats = DAMError(-1, reply->errorString(), "Failed to update categories",
                                            DAMOrigin( WindowDescriptors::_non_window_DAM, requestMap["getAllCats"]));
        cats_busy = false;
        emit localCheckIn();
        return;
    }

    QString qreply =  QString(reply->readAll());
    DAMError errCheck = scanForErrorMessage(qreply);
    if(errCheck.errorCode != 0)
    {
        status_report.last_cats = false;
        status_report.error_cats = errCheck;
        status_report.error_cats.origin_of_request.caller = WindowDescriptors::_non_window_DAM;
        status_report.error_cats.origin_of_request.request = requestMap["getAllCats"];
        cats_busy = false;
        emit localCheckIn();
        return;
    }

    existingCats.clear();
    existingCats = jsonToCatData(qreply);

    if (!status_report.init_cats)
        status_report.init_cats = true;

    cats_busy = false;
    status_report.last_cats = true;
    emit localCheckIn();
}

void DataAccessManager::performSpecificQuery(DAMOrigin info)
{
    if(info.isFullUpdate)
    {
        qDebug() << "JOSH - DON'T USE THIS TO TRIGGER A FULL UPDATE";
        return;
    }

    lastRequest = info;
    status_report.error_other.origin_of_request = info;

    alienDataType = info.format;
    if(info.isGet)
    {
        alienManager->get(QNetworkRequest(info.request));
    }
    else
    {

        qDebug() << "\n\n\n\n\t\tSOMETHING COMPLETLY UNTESTED IS ABOUT TO HAPPEN...............\n";

        alienManager->post(QNetworkRequest(lastRequest.request),
                           lastRequest.postQuery.toString(QUrl::FullyEncoded).toUtf8());
    }

}

// Return of query that was issued by something external to DAM
void DataAccessManager::alienResponse(QNetworkReply *reply)
{
    if (reply->error())
    {
        // Error origin set prior to call
        status_report.error_other = DAMError(-1, reply->errorString(), "Failed to get specific query response",
                                             status_report.error_other.origin_of_request);
        return;
    }

    QString qreply =  QString(reply->readAll());
    DAMError errCheck = scanForErrorMessage(qreply);
    DAMAlienPackage returnPackage;

    if(errCheck.errorCode != 0)
    {
        status_report.error_other.errorCode = errCheck.errorCode;
        status_report.error_other.message = errCheck.message;
        status_report.error_other.description = errCheck.description;
        emit networkError(status_report);
        return;
    }

    switch(alienDataType){
    case respDataTypes::items:
        returnPackage.items = jsonToItemData(qreply);
        break;
    case respDataTypes::reminders:
        returnPackage.rem = jsonToRemData(qreply);
        break;
    case respDataTypes::reservations:
        returnPackage.res = jsonToResData(qreply);
        break;
    case respDataTypes::schedules:
        returnPackage.sched = jsonToSchedData(qreply);
        break;
    }
    emit externalRequestResponse(errCheck, returnPackage);
}

DAMError DataAccessManager::scanForErrorMessage(QString data)
{
    DAMError e; e.errorCode = 0;
    QJsonDocument jsond = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray jsona = jsond.array();
    foreach (const QJsonValue &v, jsona)
    {
        QJsonObject err = v.toObject();
        if(err.contains("error"))
        {
            e.message = err["error"].toString();
            e.description = err["message"].toString();
            e.errorCode = -1;
        }
    }
    return e;
}

/*
    Parse data to JSON - items
*/
std::vector<reservableItems> DataAccessManager::jsonToItemData(QString data)
{
    std::vector<reservableItems> parsed;
    QJsonDocument jsond = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray jsona = jsond.array();

    // Add each item in the inv list to existing items vector
    foreach (const QJsonValue &v, jsona)
    {
        reservableItems t_item;
        QJsonObject item = v.toObject();

        t_item.barcode = item["barcode"].toString();
        t_item.name = item["name"].toString();
        t_item.desc = item["desc"].toString();

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
        parsed.push_back(t_item);
    }
    return parsed;
}


/*
    Parse data to JSON - reservations
*/
std::vector<reservations> DataAccessManager::jsonToResData(QString data)
{
    std::vector<reservations> parsed;
    QJsonDocument jsond = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray jsona = jsond.array();

    foreach (const QJsonValue &v, jsona)
    {
        reservations t_res;
        QJsonObject res = v.toObject();
        t_res.id = res["id"].toString();
        t_res.ti = res["ti"].toString();
        t_res.title = res["title"].toString();
        t_res.created = res["created"].toString();
        t_res.wfor = res["wfor"].toString();
        t_res.by = res["by"].toString();
        t_res.start = res["start"].toString();
        t_res.end = res["end"].toString();
        t_res.status = res["status"].toString();
        // Add barcodes!
        QJsonArray itemBarcodes = res["itemBarcodes"].toArray();
        foreach(const QJsonValue &i, itemBarcodes)
        {
            t_res.itemBarcodes.append(i.toString());
        }
        parsed.push_back(t_res);
    }
    return parsed;
}


/*
    Parse data to JSON - reminders
*/
std::vector<reservedReminders> DataAccessManager::jsonToRemData(QString data)
{
    std::vector<reservedReminders> parsed;
    QJsonDocument jsond = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray jsona = jsond.array();

    foreach (const QJsonValue &v, jsona)
    {
        reservedReminders t_rem;
        QJsonObject rem = v.toObject();
        t_rem.id = rem["id"].toString();
        t_rem.ti = rem["ti"].toString();
        t_rem.title = rem["title"].toString();
        t_rem.created = rem["created"].toString();
        t_rem.desc = rem["desc"].toString();
        t_rem.start = rem["start"].toString();
        t_rem.end = rem["end"].toString();
        t_rem.by = rem["by"].toString();
        t_rem.status = rem["status"].toString();
        parsed.push_back(t_rem);
    }
    return parsed;
}

std::vector<itemCategories> DataAccessManager::jsonToCatData(QString data)
{
    std::vector<itemCategories> parsed;
    QJsonDocument jsond = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray jsona = jsond.array();

    foreach (const QJsonValue &v, jsona)
    {
        itemCategories t_rem;
        QJsonObject rem = v.toObject();
        t_rem.id = rem["id"].toString();
        t_rem.label = rem["label"].toString();
        t_rem.parent = rem["parent"].toString();

        QJsonArray catbars = rem["itembarcodes"].toArray();
        foreach(const QJsonValue &i, catbars)
        {
            t_rem.barcodes.append(i.toString());
        }

        parsed.push_back(t_rem);
    }
    return parsed;
}

std::vector<schedule> DataAccessManager::jsonToSchedData(QString data)
{
    std::vector<schedule> parsed;
    QJsonDocument jsond = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray jsona = jsond.array();

    foreach (const QJsonValue &v, jsona)
    {
        schedule t_sched;
        QJsonObject sch = v.toObject();
        t_sched.scheduleid = sch["id"].toString();
        QJsonArray itemSched = sch["schedule"].toArray();
        foreach(const QJsonValue &i, itemSched)
        {
            scheduleEntry t_se;
            QJsonObject se = i.toObject();
            t_se.sid = se["sid"].toString();
            t_se.resid = se["resid"].toString();
            t_se.start = se["start"].toString();
            t_se.end = se["end"].toString();
            t_sched.scheduleInformation.push_back(t_se);
        }
        parsed.push_back(t_sched);
    }
    return parsed;
}

/*

        Data getters

*/
std::vector<reservableItems> DataAccessManager::getAllItems()
{
    return existingItems;
}
