#ifndef DATAACCESSMANAGER_H
#define DATAACCESSMANAGER_H

#include <vector>

#include <QUrl>
#include <QMap>
#include <QTime>
#include <QDebug>
#include <QString>
#include <QObject>
#include <QUrlQuery>
#include <QStringList>

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "structures.h"
#include "windowdescriptors.h"

#define WEB_SERVER_FETCH_ADDRESS "http://localhost:8080"
#define WEB_SERVER_UPDATE_ADDRESS "http://localhost:9090"
#define WEB_SERVER_APITOKEN "fefd8a1a97021dbab2d105c4784a1906cd89fc575009387d378b8807192c16e3"

enum class respDataTypes{
    items,
    schedules,
    reservations,
    reminders,
    updatedItem
};

struct DAMOrigin {
    WindowDescriptors caller;
    QUrl request;
    bool isFullUpdate;
    bool isGet, allowReplay;
    respDataTypes format;
    QUrlQuery postQuery;

    DAMOrigin()
    {

    }

    DAMOrigin(WindowDescriptors wcaller, QUrl req)
    {
        allowReplay = true;
        caller = wcaller; request = req;
    }

    DAMOrigin(WindowDescriptors w, QUrl req, bool fullUpdate, bool get, respDataTypes fmt, QUrlQuery post_query)
    {
        allowReplay = true;
        caller= w; request = req; isFullUpdate = fullUpdate, isGet = get; format = fmt; postQuery = post_query;
    }

    DAMOrigin& operator=(const DAMOrigin &other)
    {
        caller = other.caller;
        request = other.request;
        isFullUpdate = other.isFullUpdate;
        isGet = other.isGet;
        format = other.format;
        postQuery = other.postQuery;
        return *this;
    }
};

struct DAMError {
    int errorCode;
    QString message;
    QString description;
    DAMOrigin origin_of_request;

    DAMError()
    {
        errorCode = 0; message = "None"; description = "None";
    }

    DAMError(int e, QString mess, QString desc, DAMOrigin origin)
    {
        errorCode = e; message = mess, description = desc;
        origin_of_request = origin;
    }

    DAMError& operator=(const DAMError &other)
    {
        errorCode = other.errorCode;
        message = other.message;
        description = other.description;
        origin_of_request = other.origin_of_request;
        return *this;
    }
    bool operator==(const DAMError &other)
    {
        return ((errorCode == other.errorCode) &&
                (origin_of_request.caller == other.origin_of_request.caller));
    }
};

struct DAMValidUpdate {
    QString title, message;
};

struct DAMAlienPackage {
    std::vector<reservableItems> items;
    std::vector<reservations> res;
    std::vector<reservedReminders> rem;
    std::vector<schedule> sched;
    std::vector<DAMValidUpdate> update;
};

struct DAMStatus {
    bool replayable;
    bool init_item, init_res, init_rem, init_cats;
    bool last_item, last_res, last_rem, last_cats;
    QTime time_init, time_last_update;
    DAMError error_item, error_res, error_rem, error_cats, error_other;
    std::vector<DAMError> secondaryErrors;
    std::vector<DAMAlienPackage> secondaryPackages;

    DAMStatus()
    {
        DAMOrigin noOrigin;
        replayable = true;
        init_item = true; init_res = true; init_rem = true; init_cats = true;
        last_item = true; last_res = true; last_rem = true; last_cats = true;
        time_init = QTime::currentTime(); time_last_update = QTime::currentTime();
        error_item = DAMError(0, "None", "None", noOrigin);
        error_res = DAMError(0, "None", "None", noOrigin);
        error_rem = DAMError(0, "None", "None", noOrigin);
        error_cats = DAMError(0, "None", "None", noOrigin);
        error_other = DAMError(0, "None", "None", noOrigin);
    }
    DAMStatus(std::vector<DAMError> secondaryOriginErrors, std::vector<DAMAlienPackage> originPackages)
    {
        secondaryErrors = secondaryOriginErrors;
        secondaryPackages = originPackages;
    }

    DAMStatus& operator=(const DAMStatus &other)
    {
        init_item = other.init_item; init_res = other.init_res;
        init_rem = other.init_rem;
        last_item = other.last_item; last_res = other.last_res;
        last_rem = other.last_rem;
        time_init = other.time_init; time_last_update = other.time_last_update;
        error_item = other.error_item; error_res = other.error_res;
        error_rem = other.error_rem; error_other = other.error_other;
        error_cats = other.error_cats;
        secondaryErrors = other.secondaryErrors;
        secondaryPackages = other.secondaryPackages;
        return *this;
    }

    void reset()
    {
        DAMOrigin noOrigin;
        secondaryErrors.clear();
        secondaryPackages.clear();
        error_item = DAMError(0, "None", "None", noOrigin);
        error_res = DAMError(0, "None", "None", noOrigin);
        error_rem = DAMError(0, "None", "None", noOrigin);
        error_cats = DAMError(0, "None", "None", noOrigin);
        error_other = DAMError(0, "None", "None", noOrigin);
    }

};

class DataAccessManager : public QObject
{
    Q_OBJECT
public:
    explicit DataAccessManager(QObject *parent = 0);
    ~DataAccessManager();

    void updateLocalInformation(WindowDescriptors caller);

    void replayLastRequest();

    void performSpecificQuery(DAMOrigin info);


    /*
        Getters
    */
    std::vector<reservableItems> getAllItems();
    reservableItems getItemByBarcode(QString barcode);
    std::vector<itemCategories> getAllCats();

    /*
        Data Updating
    */

    // Update Item Peripherals
    DAMError updatePeripheralInformationByBarcode(QString barcode, std::vector<peripherals> newPerifs);


    /*
        Setters - Cached from Ephimeral
    */
    void cacheItemSchedules(std::vector<schedule> scheds);

signals:

    // Tie this to something to report when the network goes wrong
    // so people dont think that the program is broken...
    void networkError(DAMStatus);

    // Tell others when all data has been updated
    void dataUpdated();

    // Keep track of the locak progress
    void localCheckIn();

    // Aliens return
    void externalRequestResponse(DAMError, DAMAlienPackage);

    // Item structures have been updated
    void existingItemDataAltered(std::vector<reservableItems>);


private slots:

    // For keeping track of our progress
    void localNetCheck();

    // For populating local structures
    void updateExistingItems(QNetworkReply *reply);
    void updateTodaysReservations(QNetworkReply *reply);
    void updateTodaysReminders(QNetworkReply *reply);
    void updateCategories(QNetworkReply *reply);

    void alienResponse(QNetworkReply *reply);

private:

    bool runningFullUpdate;
    unsigned localTicker;
    DAMStatus status_report;

    QString apiToken;
    QMap<QString, QUrl> requestMap;
    DAMOrigin lastRequest;

    bool item_busy, res_busy, rem_busy, cats_busy;
    QNetworkAccessManager *itemManager;
    QNetworkAccessManager *resManager;
    QNetworkAccessManager *remManager;
    QNetworkAccessManager *catManager;

    respDataTypes alienDataType;
    QNetworkAccessManager *alienManager;

    DAMError scanForErrorMessage(QString data);

    std::vector<reservableItems> existingItems;
    std::vector<reservations> todaysReservations;
    std::vector<reservedReminders> todaysReminders;
    std::vector<itemCategories> existingCats;

    std::vector<reservableItems> jsonToItemData(QString data);
    std::vector<reservations> jsonToResData(QString data);
    std::vector<reservedReminders> jsonToRemData(QString data);
    std::vector<itemCategories> jsonToCatData(QString data);
    std::vector<schedule> jsonToSchedData(QString data);
    std::vector<DAMValidUpdate> jsonToUpData(QString data);
};

#endif // DATAACCESSMANAGER_H
