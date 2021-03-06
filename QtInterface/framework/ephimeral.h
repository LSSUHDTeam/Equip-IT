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
    BuildingReminder,
    EditingReservation,
    EditingReminder
};

class Ephimeral : public QWidget
{
    Q_OBJECT
public:
    explicit Ephimeral(ContextManager *context, EphimeralStage stage, QWidget *parent = 0);
    ~Ephimeral();

    void clearReservation();

    /*
        Reservation - Setters
    */
    void addItemToReservation(QString barcode);
    void removeItemFromReservationByBarcode(QString barcode);
    void setReservationTimeRange(QDateTime start, QDateTime end);
    void setReservationLocation(QString building, QString room);
    void setReservationFor(QString);
    void setReservationEmail(QString);
    void setReservationDescription(QString);
    void updateReservationStartTime(QDateTime newStart);
    void updateReservationEndTime(QDateTime newEnd);
    void createRepeatedObject(repetition);
    void clearReservationItems();

    /*
        Reservation - Getters
    */
    reservations getCurrentReservation();
    std::vector<reservableItems> getItemsOnReservation();
    std::vector<scheduleConflict> getReservationConflicts();
    int numberOfConflicts();
    QString getItemNameFromBarcode(QString barcode);
    QDateTime getStart();
    QDateTime getEnd();
    QString getStartString();
    QString getEndString();


    /*
        Info getters
    */
    timespecificItems getTimeSpecifiedItems();

    /*
        Check for errors and finialize
    */
    void finalizeReservation();

    /*
        Server pushes - after the emittion of
        'validReservation' by checkout.cpp
        under 'reservationIsValid()'
    */
    void submitCompletedReservation();

signals:
    // Emitted to self to stop a block
    void preparedReturned();

    // Thown when done checking for scheduling errors
    void validReservation();
    void invalidReservation(std::vector<scheduleConflict>);

    // User wants to ignore network errors
    void userMarkedIgnoreNetworkErrors();

    // Emitted once valid reservation is submitted, or fails
    void submitSuccess();

public slots:
    void preparedReturnedFromContext(std::vector<DAMError> errors, std::vector<DAMAlienPackage> packages);

    void resendPreparedPackages();
    void ignoreNetworkError();

private:
    ContextManager *localContext;
    EphimeralStage currentStage;
    timespecificItems timeScheduleCache;
    repetition repetitionFormat;
    bool repeatFlag;
    /*
        General helpers for class
    */
    QString dateTimeToString(QDateTime dt);
    QDateTime stringToDateTime(QString dt);
    QString generateTimeIndex(QString startDateTime);

    /*
        Reservation building
    */
    reservations currentReservation;
    reservedReminders currentReminder;

    /*
        Schedule Pulling
    */
    void throwNetworkErrors(NetworkCallerConfig);
    std::vector<DAMError> errorReports;
    std::vector<DAMAlienPackage> alienPackages;
    void updateScheduleCache();

    /*
        Check for schedule issues
    */
    std::vector<scheduleConflict> existingConflicts;

    /*
        Network call
    */
    void executePreparedCalls(std::vector<DAMOrigin> calls);
};

#endif // EPHIMERAL_H
