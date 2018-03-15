#include "contextmanger.h"

ContextManager::ContextManager(QObject *parent) : QObject(parent)
{
    preparedCalls = 0;
    connect(&session, SIGNAL(triggerSessionTimeout()), this, SLOT(sessionTimeout()));
    connect(&session, SIGNAL(sessionTicker(QString)), this, SLOT(sessionTicker(QString)));

    connect(&dam, SIGNAL(dataUpdated()), this, SLOT(dataReady()));
    connect(&dam, SIGNAL(externalRequestResponse(DAMError, DAMAlienPackage)),
            this, SLOT(preparedRequestResponse(DAMError, DAMAlienPackage)));

    connect(&dam, SIGNAL(networkError(DAMStatus)), this, SLOT(networkError(DAMStatus)));
}

bool ContextManager::createSession(QString uid)
{
    return session.startSession(uid);
}

void ContextManager::addUserCrumb(QString actionDescription, bool silent)
{

    session.addCrumb(actionDescription, silent);
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

std::vector<reservableItems> ContextManager::getExistingItems()
{
    return dam.getAllItems();
}

reservableItems ContextManager::getItemByBarcode(QString barcode)
{
    return dam.getItemByBarcode(barcode);
}

std::vector<itemCategories> ContextManager::getExistingCats()
{
    return dam.getAllCats();
}

std::vector<schedule> ContextManager::getScheduleCache()
{
    return schedCache;
}

bool ContextManager::isSchedCacheValid()
{
    return schedCache.size() > 0;
}

DAMError ContextManager::updateItemPeriphs(QString barcode, std::vector<peripherals> newPeriphs)
{
    return dam.updatePeripheralInformationByBarcode(barcode, newPeriphs);
}

void ContextManager::cacheScheduleData(std::vector<schedule> data)
{
    schedCache = data;
}

void ContextManager::clearSchedCache()
{
    schedCache.clear();
}

void ContextManager::prepareNetworkCalls(std::vector<DAMOrigin> calls, WindowDescriptors wcaller)
{
    preparedErrors.clear();
    preparedResponses.clear();
    preparedCalls = calls.size();
    currentNetworkCaller = wcaller;

    if(DISPLAY_TESTING_FEATURES)
        qDebug() << "ContextManager: Preparing [" <<
                    preparedCalls << "] calls from " <<
                    winHelp.getStringFromWid(wcaller) ;

    foreach(DAMOrigin call, calls)
    {
        dam.performSpecificQuery(call);
    }
}

void ContextManager::preparedRequestResponse(DAMError e, DAMAlienPackage p)
{
    preparedCalls--;

    if(DISPLAY_TESTING_FEATURES)
        qDebug() << "ContextManager: Got prepared response. [" <<
                    preparedCalls << "] response(s) left.";

    if(e.errorCode != 0)
    {
        preparedErrors.push_back(e);
    } else {
        preparedResponses.push_back(p);
    }

    if(preparedCalls == 0)
    {
        emit preparedDataReady(preparedErrors,preparedResponses);
    }
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

void ContextManager::itemDataAltered(std::vector<reservableItems> items)
{
    emit rippleDAMItemDataAltered(items);
}

void ContextManager::killSession()
{
    session.killSession();
}
