#include "contextmanger.h"

ContextManager::ContextManager(QObject *parent) : QObject(parent)
{
    connect(&session, SIGNAL(triggerSessionTimeout()), this, SLOT(sessionTimeout()));
    connect(&session, SIGNAL(sessionTicker(QString)), this, SLOT(sessionTicker(QString)));
}

bool ContextManager::createSession(QString uid)
{
    return session.startSession(uid);
}

void ContextManager::addUserCrumb(QString actionDescription)
{
    session.addCrumb(actionDescription);
}

void ContextManager::changeUserLocation(WindowDescriptors location)
{
    session.changeWindow(location);
}

QString ContextManager::getSessionUser()
{
    return session.getUserId();
}

void ContextManager::sessionTimeout()
{
    emit rippleSessionTimeout();
}

void ContextManager::sessionTicker(QString timeRemaining)
{
    emit rippleSessionTicker(timeRemaining);
}

void ContextManager::killSession()
{
    session.killSession();
}
