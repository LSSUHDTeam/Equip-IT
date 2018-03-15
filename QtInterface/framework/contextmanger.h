#ifndef CONTEXTMANGER_H
#define CONTEXTMANGER_H

#include <QDebug>
#include <QObject>
#include "sessionobject.h"
#include "dataaccessmanager.h"
#include "windowdescriptors.h"
#include <vector>

class ContextManager : public QObject
{
    Q_OBJECT
public:
    explicit ContextManager(QObject *parent = 0);


    /*
        Context Getters
    */
    std::vector<schedule> getScheduleCache();
    bool isSchedCacheValid();

    /*
        Context Setters
    */
    void cacheScheduleData(std::vector<schedule> data);
    void clearSchedCache();

    /*
        Session Accessors
    */
    bool createSession(QString uid);
    void addUserCrumb(QString actionDescription, bool silent=false);
    void changeUserLocation(WindowDescriptors location);

    // End a current session
    void killSession();

    /*
        Session Getters
    */
    QString getSessionUser();

    /*
        DAM Network Accessors
    */
    void replayRequest();

    /*
        DAM Getters
    */
    std::vector<reservableItems> getExistingItems();
    reservableItems getItemByBarcode(QString barcode);
    std::vector<itemCategories> getExistingCats();

    /*
        DAM Local Data Setters
    */
    DAMError updateItemPeriphs(QString barcode, std::vector<peripherals> newPeriphs);


    /*
        Prepare specific number of requests, and emit
        back to caller once all are taken care of
    */
    void prepareNetworkCalls(std::vector<DAMOrigin> calls, WindowDescriptors callerId);

signals:

    // From session object
    void rippleSessionTimeout();
    void rippleSessionTicker(QString);

    // From DAM object
    void rippleDataReady();
    void rippleNetworkError(DAMStatus);

    void rippleDAMItemDataAltered(std::vector<reservableItems>);

    // From self
    void preparedDataReady(std::vector<DAMError>, std::vector<DAMAlienPackage>);

public slots:

    // Ripple session timeout
    void sessionTimeout();
    void sessionTicker(QString);

    // Ripple DAM
    void dataReady();
    void networkError(DAMStatus);
    void preparedRequestResponse(DAMError, DAMAlienPackage);
    void itemDataAltered(std::vector<reservableItems>);

private:
    SessionObject session;
    DataAccessManager dam;
    WindowHelper winHelp;
    std::vector<schedule> schedCache;

    WindowDescriptors currentNetworkCaller;
    int preparedCalls;
    std::vector<DAMError> preparedErrors;
    std::vector<DAMAlienPackage> preparedResponses;

};

#endif // CONTEXTMANGER_H
