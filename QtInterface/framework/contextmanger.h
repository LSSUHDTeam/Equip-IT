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
        Data Access Manager Accessors
    */

    /*
        Data Access Manager Getters
    */

signals:

    // From session object
    void rippleSessionTimeout();
    void rippleSessionTicker(QString);


public slots:

    // Ripple session timeout
    void sessionTimeout();
    void sessionTicker(QString);

private:
    SessionObject session;
    DataAccessManager dam;

};

#endif // CONTEXTMANGER_H
