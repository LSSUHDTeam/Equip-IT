#include "ephimeral.h"

Ephimeral::Ephimeral(ContextManager *context, EphimeralStage stage,  QWidget *parent) : QWidget(parent)
{
    localContext = context;
    currentStage = stage;

    connect(localContext, SIGNAL(preparedDataReady(std::vector<DAMError>, std::vector<DAMAlienPackage>)),
            this, SLOT(preparedReturnedFromContext(std::vector<DAMError>, std::vector<DAMAlienPackage>)));

    if(!localContext->isSchedCacheValid())
        updateScheduleCache();

    repeatFlag = false;
    switch(stage){
    case EphimeralStage::BuildingReminder:
        localContext->addUserCrumb("Ephimeral: Creating new reminder");
        break;
    case EphimeralStage::BuildingReservation:
        localContext->addUserCrumb("Ephimeral: Building new reservation");
        break;
    case EphimeralStage::EditingReminder:
        localContext->addUserCrumb("Ephimeral: Editing a reminder");
        break;
    case EphimeralStage::EditingReservation:
        localContext->addUserCrumb("Ephimeral: Editing a reservation");
        break;
    }
}

Ephimeral::~Ephimeral()
{
    localContext = 0;
}

// Update the schedules of items that exist
void Ephimeral::updateScheduleCache()
{
    // Generate network calls to get ALL item's schedules
    std::vector<DAMOrigin> preparedCalls;
    std::vector<reservableItems> allItems = localContext->getExistingItems();
    for(auto i = allItems.begin(); i != allItems.end(); ++i)
    {
        DAMOrigin call(
                    WindowDescriptors::_non_window_ephimeral,
                    QUrl( QString(WEB_SERVER_FETCH_ADDRESS)
                          + "/schedules?classifier=single&id=" + (*i).barcode +
                         "&token=" + QString(WEB_SERVER_APITOKEN))
                    );
        call.isGet = true;
        call.allowReplay = false;
        call.isFullUpdate = false;
        call.format = respDataTypes::schedules;
        preparedCalls.push_back(call);
    }

    // Make the calls
    executePreparedCalls(preparedCalls);

    // Check for network errors
    if(errorReports.size() > 0)
    {
        throwNetworkErrors(NetworkCallerConfig(NetworkCallerOrigin::secondary, DAMStatus(errorReports,alienPackages),true));
        return;
    }

    std::vector<schedule> scheduleCache;
    for(auto i = alienPackages.begin(); i != alienPackages.end(); ++i)
    {
        for(auto j = (*i).sched.begin(); j != (*i).sched.end(); ++j)
        {
            scheduleCache.push_back((*j));
        }
    }
    localContext->cacheScheduleData(scheduleCache);
    return;
}

/*
    Reservation - Setters
*/
void Ephimeral::setReservationTimeRange(QDateTime start, QDateTime end)
{
    timeScheduleCache.clear();
    currentReservation.start = dateTimeToString(start);
    currentReservation.end = dateTimeToString(end);

    qDebug() << "Reservation start: " << currentReservation.start
             << "\nReservation end: " << currentReservation.end;
}

void Ephimeral::addItemToReservation(QString barcode)
{
    // Get out of here if you're not unique!
    foreach(const QString &item, currentReservation.itemBarcodes)
        if(item == barcode)
            return;
    currentReservation.itemBarcodes.append(barcode);
}

void Ephimeral::removeItemFromReservationByBarcode(QString barcode)
{
    QStringList newItemList;
    foreach(QString item, currentReservation.itemBarcodes)
        if(item != barcode)
        {
            newItemList.append(item);
        }

    currentReservation.itemBarcodes.clear();
    currentReservation.itemBarcodes = newItemList;
}

void Ephimeral::setReservationLocation(QString building, QString room)
{
    currentReservation.title = building + " in " + room;
}

void Ephimeral::setReservationFor(QString name)
{
    currentReservation.wfor = name;
}

void Ephimeral::setReservationEmail(QString email)
{
    currentReservation.email = email;
}

void Ephimeral::setReservationDescription(QString desc)
{
    currentReservation.title = desc;
}

void Ephimeral::updateReservationStartTime(QDateTime newStart)
{
    currentReservation.start = dateTimeToString(newStart);
}

void Ephimeral::updateReservationEndTime(QDateTime newEnd)
{
    currentReservation.end = dateTimeToString(newEnd);
}

void Ephimeral::createRepeatedObject(repetition info)
{
    repetitionFormat = info;
    repeatFlag = true;
    if(timeScheduleCache.inited)
        timeScheduleCache.clear();
}

void Ephimeral::clearReservationItems()
{
    currentReservation.itemBarcodes.clear();
}

reservations Ephimeral::getCurrentReservation()
{
    return currentReservation;
}

std::vector<reservableItems> Ephimeral::getItemsOnReservation()
{
    std::vector<reservableItems> itemsOnRes;
    std::vector<reservableItems> allItems = localContext->getExistingItems();
    foreach(QString barcode, currentReservation.itemBarcodes)
    {
        for(auto i = allItems.begin(); i != allItems.end(); ++i)
        {
            if((*i).barcode == barcode)
                itemsOnRes.push_back((*i));
        }
    }
    return itemsOnRes;
}

std::vector<scheduleConflict> Ephimeral::getReservationConflicts()
{
    return existingConflicts;
}

int Ephimeral::numberOfConflicts()
{
    return existingConflicts.size();
}

QString Ephimeral::getItemNameFromBarcode(QString barcode)
{
    std::vector<reservableItems> items = localContext->getExistingItems();
    for(auto i = items.begin(); i != items.end(); ++i)
    {
        if((*i).barcode == barcode)
        {
            return (*i).name;
        }
    }
    return "Barcode not located\nTry updating.";
}

QDateTime Ephimeral::getStart()
{
    return stringToDateTime(currentReservation.start);
}

QDateTime Ephimeral::getEnd()
{
    return stringToDateTime(currentReservation.end);
}

QString Ephimeral::getStartString()
{
    return currentReservation.start;
}

QString Ephimeral::getEndString()
{
    return currentReservation.end;
}

timespecificItems Ephimeral::getTimeSpecifiedItems()
{
    if(timeScheduleCache.inited)
        return timeScheduleCache;

    existingConflicts.clear();
    std::vector<schedule> cached = localContext->getScheduleCache();
    std::vector<reservableItems> conflictingItems, nonconflictingItems;
    std::vector<schedule> conflictingSchedules, nonConflictingSchules;
    for(auto i = cached.begin(); i != cached.end(); ++i)
    {
        scheduleConflict temp = (*i).checkForConflict(
                    currentReservation.start, currentReservation.end
                    );
        if(temp.exists)
        {
            // If its relevant to our reservation, now then we have an issue
            if(currentReservation.itemBarcodes.contains((*i).scheduleid))
                existingConflicts.push_back(temp);

            conflictingItems.push_back(localContext->getItemByBarcode(temp.itembarcode));
            conflictingSchedules.push_back((*i));
        } else {
            nonconflictingItems.push_back(localContext->getItemByBarcode(temp.itembarcode));
            nonConflictingSchules.push_back((*i));
        }
    }
    timeScheduleCache = timespecificItems(
                nonconflictingItems, conflictingItems,
                nonConflictingSchules, conflictingSchedules
                );
    return timeScheduleCache;
}

void Ephimeral::finalizeReservation()
{
    timeScheduleCache.clear();
    getTimeSpecifiedItems();

    if(existingConflicts.size()>0)
    {
        emit invalidReservation(existingConflicts);
    } else {
        emit validReservation();
    }
}


void Ephimeral::submitCompletedReservation()
{
    if(currentStage == EphimeralStage::BuildingReservation)
    {
        currentReservation.id = "NEW";
        currentReservation.retby = "None";
        currentReservation.created = "Set by server";
        currentReservation.status = "incomplete";
        currentReservation.ti = generateTimeIndex(currentReservation.start);
        currentReservation.by = localContext->getSessionUser();
    }

    QUrl outUrl = QUrl(QString(WEB_SERVER_UPDATE_ADDRESS) +
                  "/upres?token=" + QString(WEB_SERVER_APITOKEN) +
                       "&data=" + currentReservation.exportJson());

    // Prepare the network call
    std::vector<DAMOrigin> preparedCalls;
    DAMOrigin call(WindowDescriptors::_non_window_ephimeral,outUrl);
    call.isGet = true;
    call.allowReplay = false;
    call.isFullUpdate = false;
    call.format = respDataTypes::updatedItem;
    preparedCalls.push_back(call);

    foreach(QString itemBarcode, currentReservation.itemBarcodes)
    {
        reservableItems currentItem = localContext->getItemByBarcode(itemBarcode);

        if(currentItem.barcode != "error")
        {
            QUrl outUrl = QUrl(QString(WEB_SERVER_UPDATE_ADDRESS) +
                          "/upitem?token=" + QString(WEB_SERVER_APITOKEN) +
                               "&data=" + currentItem.exportJson());

            DAMOrigin call(WindowDescriptors::_non_window_ephimeral,outUrl);
            call.isGet = true;
            call.allowReplay = false;
            call.isFullUpdate = false;
            call.format = respDataTypes::updatedItem;
            preparedCalls.push_back(call);
        }
    }

    // Execute
    executePreparedCalls(preparedCalls);

    // If errors exist, we want to throw them at the user.
    // Once in their face, they may choose to retry the
    // connection... or they can ignore it completly.
    if(errorReports.size() > 0)
    {
        throwNetworkErrors(NetworkCallerConfig(NetworkCallerOrigin::secondary, DAMStatus(errorReports,alienPackages),true));
    } else {
        // Clear cache because it is now invalid
        localContext->clearSchedCache();
        emit submitSuccess();
    }
}

void Ephimeral::preparedReturnedFromContext(std::vector<DAMError> errors, std::vector<DAMAlienPackage> packages)
{
    errorReports = errors;
    alienPackages = packages;
    emit preparedReturned();
}

void Ephimeral::resendPreparedPackages()
{
    finalizeReservation();
}

void Ephimeral::ignoreNetworkError()
{
    emit userMarkedIgnoreNetworkErrors();
}

void Ephimeral::executePreparedCalls(std::vector<DAMOrigin> calls)
{
    errorReports.clear();
    alienPackages.clear();

    // Call DAM to request
    localContext->prepareNetworkCalls(calls, WindowDescriptors::_non_window_ephimeral);

    // Block until data returned
    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop blockLoop;
    connect(this,  SIGNAL(preparedReturned()), &blockLoop, SLOT(quit()) );
    connect(&timer, SIGNAL(timeout()), &blockLoop, SLOT(quit()));
    qDebug() << "Attempting to pull all schedules. Timeout after 20 seconds.";
    timer.start(20000);
    blockLoop.exec();

    // If the loop is broken, and the timer isn't active still, then we waited too long.
    if(!timer.isActive())
    {
        errorReports.push_back(
                    DAMError(-1,
                      "Network Timeout (20 Seconds)!",
                      "The network has timed out while trying to receive item schedules from the server.",
                             DAMOrigin(WindowDescriptors::_non_window_ephimeral, QUrl("")))
                    );
    }
}

void Ephimeral::throwNetworkErrors(NetworkCallerConfig callerConfig)
{
    // Setup 'secondary' network status report - meaning that they can't do a retry,
    // and that DAMStatus didn't originate from the actual Data Access Manager
    DisplayNetworkError *dne = new DisplayNetworkError(localContext,callerConfig, this);
    connect(dne, SIGNAL(retrying()), this, SLOT(resendPreparedPackages()));
    connect(dne, SIGNAL(ignoreIssue()), this, SLOT(ignoreNetworkError()));
    dne->setAttribute(Qt::WA_DeleteOnClose, true);
    dne->show();
}

/*
    Helpers
*/
QString Ephimeral::dateTimeToString(QDateTime dt)
{
    return dt.toString(QString(DATETIME_FORMAT));
}

QDateTime Ephimeral::stringToDateTime(QString dt)
{
    return QDateTime::fromString(dt, QString(DATETIME_FORMAT));
}

QString Ephimeral::generateTimeIndex(QString startDateTime)
{
    QDateTime sdt = stringToDateTime(startDateTime);
    return sdt.toString(QString(DB_TIME_INDEX));
}

