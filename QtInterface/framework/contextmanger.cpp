#include "contextmanger.h"

ContextManager::ContextManager(QObject *parent) : QObject(parent)
{
    connect(&session, SIGNAL(triggerSessionTimeout()), this, SLOT(sessionTimeout()));
    connect(&session, SIGNAL(sessionTicker(QString)), this, SLOT(sessionTicker(QString)));

    connect(&dam, SIGNAL(dataUpdated()), this, SLOT(dataReady()));
    connect(&dam, SIGNAL(networkError(DAMStatus)), this, SLOT(networkError(DAMStatus)));
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

void ContextManager::replayRequest()
{
    dam.replayLastRequest();
}

void ContextManager::performSpecifiedQuery(DAMOrigin queryInfo)
{
    dam.performSpecificQuery(queryInfo);
}

std::vector<reservableItems> ContextManager::getExistingItems()
{
    return dam.getAllItems();
}

void ContextManager::sessionTimeout()
{
    emit rippleSessionTimeout();
}

void ContextManager::sessionTicker(QString timeRemaining)
{
    emit rippleSessionTicker(timeRemaining);
}

void ContextManager::dataReady()
{
    emit rippleDataReady();
}

void ContextManager::networkError(DAMStatus e)
{
    emit rippleNetworkError(e);
}

void ContextManager::externalRequestResponse(DAMError e, DAMAlienPackage p)
{
    emit rippleExternalRequestResponse(e,p);
}

void ContextManager::killSession()
{
    session.killSession();
}
