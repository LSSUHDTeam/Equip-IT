#include "sessionobject.h"

SessionObject::SessionObject(QObject *parent) : QObject(parent)
{
    // All users start on the login window
    currentPath.window = WindowDescriptors::LoginWindow;
    connect(&sessionTimer, SIGNAL(timeout() ), this, SLOT(handleSessionTimeout()));
    connect(&constantTicker, SIGNAL(timeout() ), this, SLOT(handleSessionTick()));
}

bool SessionObject::startSession(QString uid)
{
    if (!checkId(uid))
        return false;
    userID = uid;
    addCrumb(userID + " logged in @" + QTime::currentTime().toString());
    sessionTimer.start(SESSION_LENGTH);
    constantTicker.start(1000);
    return true;
}

void SessionObject::addCrumb(QString desc, bool silent)
{
    // Add a user action to the current page
    currentPath.actionDescription.append(desc);

    // Reset sesson b/c of activity
    if(!silent)
        resetSessionTimer();
}

void SessionObject::changeWindow(WindowDescriptors wid)
{
    // Indicate the action of changing windows
    addCrumb("change window");

    // Add the window actions to the list
    crumbs.push_back(currentPath);

    // Change the window, and clear path
    currentPath.window = wid;
    currentPath.actionDescription.clear();
}

QString SessionObject::getUserId()
{
    return userID;
}

void SessionObject::handleSessionTimeout()
{
    addCrumb("Session timeout @"+QTime::currentTime().toString());
    endCurrentSession();
    emit triggerSessionTimeout();
}

void SessionObject::handleSessionTick()
{
    if (sessionTimer.isActive())
    {
        emit sessionTicker("Timeout in: " + getSessionRemainingTime());
    }
}

void SessionObject::killSession()
{
    addCrumb("User killed session");
    endCurrentSession();
}

void SessionObject::resetSessionTimer()
{
    if (sessionTimer.isActive())
    {
        sessionTimer.stop();
        sessionTimer.start(SESSION_LENGTH);
    }
}

QString SessionObject::getSessionRemainingTime()
{
    if (sessionTimer.isActive())
    {
        const double n = static_cast<double>(sessionTimer.remainingTime()) / 1000 / 60;
        return QString().setNum(n, 'g', 3);
    } else {
        return "Session not active";
    }
}

bool SessionObject::checkId(QString uid)
{
    if(uid.length() != 9)
        return false;
    if( (uid.at(0) != 'a') && (uid.at(0) != 'A' ))
        return false;
    return true;
}

void SessionObject::endCurrentSession()
{
    if (constantTicker.isActive())
        constantTicker.stop();

    if (sessionTimer.isActive())
        sessionTimer.stop();

    /*
        Store uid and crumbs to file/db
    */
    addCrumb("Session ended for " + userID + "@" + QTime::currentTime().toString());
    crumbs.push_back(currentPath);

    // Save info here
    DEBUG_show_user_session();

    // Reset back to login page info and clear
    currentPath.window = WindowDescriptors::LoginWindow;
    currentPath.actionDescription.clear();
    crumbs.clear();
}

void SessionObject::DEBUG_show_user_session()
{
    WindowHelper wh;
    for(auto i = crumbs.begin(); i != crumbs.end(); ++i)
    {
        qDebug() << wh.getStringFromWid((*i).window);
        foreach(QString item, (*i).actionDescription )
        {
            qDebug() << item << " -> ";
        }
        qDebug() << "\n";
    }
    qDebug() << "END";
}
