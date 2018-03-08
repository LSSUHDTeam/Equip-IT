#include "ephimeral.h"

Ephimeral::Ephimeral(ContextManager *context, QWidget *parent) : QWidget(parent)
{
    localContext = context;
    connect(localContext, SIGNAL(preparedDataReady(std::vector<DAMError>, std::vector<DAMAlienPackage>)),
            this, SLOT(preparedReturnedFromContext(std::vector<DAMError>, std::vector<DAMAlienPackage>)));
}

Ephimeral::~Ephimeral()
{
    localContext = 0;
}

/*
    Reservation - Setters
*/
void Ephimeral::setReservationTimeRange(QDateTime start, QDateTime end)
{
    currentReservation.start = dateTimeToString(start);
    currentReservation.end = dateTimeToString(end);
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

void Ephimeral::updateReservationStartTime(QDateTime newStart)
{
    currentReservation.start = dateTimeToString(newStart);
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

void Ephimeral::finalizeReservation()
{
    errorReports.clear();
    alienPackages.clear();

    // Get schedule of items on res list
    secureItemScheduleData();

    // If errors exist, we want to throw them at the user. Once in their face, they may choose to retry the
    // connection... or they can ignore it completly.
    if(errorReports.size() > 0)
    {
        throwNetworkErrors(NetworkCallerConfig(NetworkCallerOrigin::secondary, DAMStatus(errorReports,alienPackages),true));
    } else {

        // If no errors occur, then we need to start analyzing the schedule to check for conflicts and whatnot
        analyzeSchedule();
    }
}

void Ephimeral::secureItemScheduleData()
{
    std::vector<DAMOrigin> prepareCalls;
    foreach(QString barcode, currentReservation.itemBarcodes)
    {
        DAMOrigin call(
                    WindowDescriptors::_non_window_ephimeral,
                    QUrl( QString(WEB_SERVER_ADDRESS)
                          + "/schedules?classifier=single&id=" + barcode +
                         "&token=" + QString(WEB_SERVER_APITOKEN))
                    );
        call.isGet = true;
        call.allowReplay = false;
        call.isFullUpdate = false;
        call.format = respDataTypes::schedules;
        prepareCalls.push_back(call);
    }

    // Call DAM to request
    localContext->prepareNetworkCalls(prepareCalls, WindowDescriptors::_non_window_ephimeral);

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

void Ephimeral::analyzeSchedule()
{
    existingConflicts.clear();
    for(auto i = alienPackages.begin(); i != alienPackages.end(); ++i)
    {
        for(auto j = (*i).sched.begin(); j != (*i).sched.end(); ++j)
        {
            scheduleConflict temp = (*j).checkForConflict(currentReservation.start, currentReservation.end);
            if(temp.exists)
                existingConflicts.push_back(temp);
        }
    }

    if(existingConflicts.size()>0)
    {
        emit invalidReservation(existingConflicts);
    } else {
        emit validReservation();
    }
}

void Ephimeral::submitCompletedReservation()
{


    // Push info to the server, and then like... kill self.
    // Error handling is cool too.



    // Create request, and push. Hang until a response is sent back validating the push



    // / / / / / WIll need a mechanism to determine if we are submitting a new res
    // or if we are updating a current reservation. Ephimeral will be used for both as
    // the same rules will apply !!!




    qDebug() << "Ephimeral::submitCompletedReservation -> Submit the reservation !";
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

