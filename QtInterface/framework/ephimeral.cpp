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

void Ephimeral::addItemToReservation(ephimeralResItem item)
{
    currentReservation.itemBarcodes.append(item.barcode);
}

void Ephimeral::removeItemFromReservationByBarcode(QString barcode)
{
    currentReservation.itemBarcodes.removeOne(barcode);
}

/*
    Reservation - Getters
*/
std::vector<ephimeralResItem> Ephimeral::getAvailableItemsAndDueDateTimes(QString start, QString end)
{
    QDateTime formated_datetime_start = stringToDateTime(start);
    QDateTime formated_datetime_end = stringToDateTime(end);
}

reservations Ephimeral::getCurrentReservation()
{
    return currentReservation;
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
