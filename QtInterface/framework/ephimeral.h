/*
    Ephimeral is a structure to hold all structures being built by the user (reservations, reminders, etc)
    The ephimeral class will hold all data until either purged by the user, or pushed to the server.
    The ephimeral class handles the pushing of data to the server by-way of the data access manager
    to simplify calls.
*/

#ifndef EPHIMERAL_H
#define EPHIMERAL_H

#include <QObject>
#include <QDateTime>
#include "contextmanger.h"
#include "structures.h"

#define DATETIME_FORMAT "hh:mm. ddd MMMM d yy"

enum class EphimeralStage{
    BuildingReservation,
    BuildingReminder
};

struct ephimeralResItem{
    reservableItems resItem;
    QString barcode;
    QString nextScheduledDateTime;

    ephimeralResItem(reservableItems item, QString nextRes, QString ibarcode)
    {
        resItem = item; nextScheduledDateTime = nextRes; barcode = ibarcode;
    }
};

class Ephimeral : public QObject
{
    Q_OBJECT
public:
    explicit Ephimeral(ContextManager *context, QObject *parent = 0);
    ~Ephimeral();

    /*
        Reservation - Setters
    */
    void setReservationTimeRange(QDateTime start, QDateTime end);
    void addItemToReservation(ephimeralResItem);
    void removeItemFromReservationByBarcode(QString barcode);

    /*
        Reservation - Getters
    */
    std::vector<ephimeralResItem> getAvailableItemsAndDueDateTimes(QString start, QString end);
    reservations getCurrentReservation();

signals:
public slots:

private:
    ContextManager *localContext;

    /*
        General helpers for class
    */
    QString dateTimeToString(QDateTime dt);
    QDateTime stringToDateTime(QString dt);

    /*
        Reservation building
    */
    reservations currentReservation;



};

#endif // EPHIMERAL_H
