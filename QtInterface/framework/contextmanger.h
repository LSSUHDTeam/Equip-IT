#ifndef CONTEXTMANGER_H
#define CONTEXTMANGER_H

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
        Session Accessors
    */
    bool createSession(QString uid);
    void addUserCrumb(QString actionDescription);
    void changeUserLocation(WindowDescriptors location);

    // End a current session
    void killSession();

    /*
        Session Getters
    */
    QString getSessionUser();

    /*
        DAM Accessors
    */
    void replayRequest();
    void performSpecifiedQuery(DAMOrigin queryInfo);


    /*
        DAM Getters
    */
    std::vector<reservableItems> getExistingItems();


signals:

    // From session object
    void rippleSessionTimeout();
    void rippleSessionTicker(QString);

    // From DAM object
    void rippleDataReady();
    void rippleNetworkError(DAMStatus);
    void rippleExternalRequestResponse(DAMError, DAMAlienPackage);


public slots:

    // Ripple session timeout
    void sessionTimeout();
    void sessionTicker(QString);

    // Ripple DAM
    void dataReady();
    void networkError(DAMStatus);
    void externalRequestResponse(DAMError, DAMAlienPackage);

private:
    SessionObject session;
    DataAccessManager dam;

};

#endif // CONTEXTMANGER_H
