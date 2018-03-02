#include "ephimeral.h"

Ephimeral::Ephimeral(ContextManager *context, QObject *parent) : QObject(parent)
{
    localContext = context;
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
    currentReservation.itemBarcodes.append(barcode);
}

void Ephimeral::removeItemFromReservationByBarcode(QString barcode)
{
    currentReservation.itemBarcodes.removeOne(barcode);
}

void Ephimeral::setReservationLocation(QString building, QString room)
{
    currentReservation.title = building + " in " + room;
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
