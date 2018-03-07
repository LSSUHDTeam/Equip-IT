/*
    Ephimeral is a structure to hold all structures being built by the user (reservations, reminders, etc)
    The ephimeral class will hold all data until either purged by the user, or pushed to the server.
    The ephimeral class handles the pushing of data to the server by-way of the data access manager
    to simplify calls.
*/

#ifndef EPHIMERAL_H
#define EPHIMERAL_H

#include <QTimer>
#include <QEventLoop>
#include <QObject>
#include <QDateTime>
#include "contextmanger.h"
#include "framework/windowdescriptors.h"
#include "windows/displaynetworkerror.h"
#include "structures.h"

enum class EphimeralStage{
    BuildingReservation,
    BuildingReminder
};

class Ephimeral : public QWidget
{
    Q_OBJECT
public:
    explicit Ephimeral(ContextManager *context, QWidget *parent = 0);
    ~Ephimeral();

    void clearReservation();

    /*
        Reservation - Setters
    */
    void setReservationTimeRange(QDateTime start, QDateTime end);
    void addItemToReservation(QString barcode);
    void removeItemFromReservationByBarcode(QString barcode);
    void setReservationLocation(QString building, QString room);
    void setReservationFor(QString);
    void setReservationEmail(QString);

    /*
        Reservation - Getters
    */
    reservations getCurrentReservation();
    std::vector<reservableItems> getItemsOnReservation();
    std::vector<scheduleConflict> getReservationConflicts();
    int numberOfConflicts();
    QString getItemNameFromBarcode(QString barcode);


    /*
        Check for errors and finialize
    */
    void finalizeReservation();

signals:
    // Emitted to self to stop a block
    void preparedReturned();

    // Thown when done checking for scheduling errors
    void validReservation();
    void invalidReservation(std::vector<scheduleConflict>);

public slots:
    void preparedReturnedFromContext(std::vector<DAMError> errors, std::vector<DAMAlienPackage> packages);

    void resendPreparedPackages();
    void ignoreNetworkError();

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

    /*
        Schedule Pulling
    */
    void throwNetworkErrors(NetworkCallerConfig);
    std::vector<DAMError> errorReports;
    std::vector<DAMAlienPackage> alienPackages;
    void secureItemScheduleData();

    /*
        Check for schedule issues
    */
    std::vector<scheduleConflict> existingConflicts;
    void analyzeSchedule();

    /*
        Server pushes
    */
    void submitCompletedReservation();


};

#endif // EPHIMERAL_H
