#ifndef SESSIONOBJECT_H
#define SESSIONOBJECT_H

#include <QObject>
#include "windowdescriptors.h"
#include <QStringList>
#include <vector>
#include <QTime>
#include <QTimer>
#include <QTimerEvent>
#include <QDebug>

#define SESSION_LENGTH 300000

struct UserActionPath {
    WindowDescriptors window;
    QStringList actionDescription;
};

class SessionObject : public QObject
{
    Q_OBJECT
public:
    explicit SessionObject(QObject *parent = 0);

    // Setters / accessors
    bool startSession(QString uid);
    void addCrumb(QString actionDescription, bool silent=false);
    void changeWindow(WindowDescriptors wid);
    void killSession();


    // Getters
    QString getUserId();

signals:

    // Session has ended due to inactivity
    void triggerSessionTimeout();

    // Session tick
    void sessionTicker(QString timeRemaining);

public slots:

private slots:
    void handleSessionTimeout();

    void handleSessionTick();


private:
    QTimer sessionTimer;
    QTimer constantTicker;

    // User information, and actions taken
    QString userID;
    struct UserActionPath currentPath;
    std::vector<struct UserActionPath> crumbs;

    // Validate the user id - Internal
    bool checkId(QString uid);

    // Extend session
    void resetSessionTimer();

    // String the session time for ticking
    QString getSessionRemainingTime();

    // End the current session
    void endCurrentSession();

    // qDebug out the latest session
    void DEBUG_show_user_session();

};

#endif // SESSIONOBJECT_H
