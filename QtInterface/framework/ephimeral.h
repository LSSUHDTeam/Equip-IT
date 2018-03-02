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

class Ephimeral : public QObject
{
    Q_OBJECT
public:
    explicit Ephimeral(ContextManager *context, QObject *parent = 0);
    ~Ephimeral();

    void clearReservation();

    /*
        Reservation - Setters
    */
    void setReservationTimeRange(QDateTime start, QDateTime end);
    void addItemToReservation(QString barcode);
    void removeItemFromReservationByBarcode(QString barcode);
    void setReservationLocation(QString building, QString room);

    /*
        Reservation - Getters
    */
    reservations getCurrentReservation();
    std::vector<reservableItems> getItemsOnReservation();

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
