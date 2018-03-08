#ifndef CHECKOUT_H
#define CHECKOUT_H

#include <QDebug>
#include <QMdiArea>
#include <QMainWindow>
#include <QCloseEvent>
#include "framework/ephimeral.h"
#include "framework/contextmanger.h"
#include "dialogs/timegetter.h"
#include "dialogs/quicktimeget.h"
#include "dialogs/reservationviewer.h"
#include "dialogs/simplemessagebox.h"
#include "dialogs/finalizereservation.h"
#include "windows/screenboard.h"
#include "windows/quickscanitem.h"
#include "windows/buildingselection.h"
#include "objects/etouchlineedit.h"


enum class KeyboardFlow{
    ignore,
    who,
    email
};

enum class CheckoutType{
    QuickCheckout,
    BuilderCheckout
};

struct QCTimeWarnings{
    reservableItems item;
    bool shownOnce;
    scheduleEntry entryOfConfliction;
    QCTimeWarnings(reservableItems theItem, scheduleEntry confliction)
    {
        item = theItem;
        entryOfConfliction = confliction;
        shownOnce = false;
    }
};

namespace Ui {
class CheckOut;
}

class CheckOut : public QMainWindow
{
    Q_OBJECT

public:
    explicit CheckOut(ContextManager *context, CheckoutType mode, QWidget *parent = 0);
    ~CheckOut();

signals:
    void checkoutClosed(WindowDescriptors window);
    void closeKeyboard();
    void closeChildren();

public slots:

    // Parent call
    void forceClose();

    // Ephimeral signaled to ignore network error
    void ephimeralNetworkErrorMarkedIgnore();

    // Keyboard data slots
    void keyboardClosed();
    void keyboardDataReceived(QString data);

    // Line edit slots
    void forFocusGained();
    void forFocusLost();
    void emailFocusGained();
    void emailFocusLost();

    // Data window return slots
    void addItemsToReservation(QStringList barcodes);

    // Time frame returned
    void setTimeFrame(QDateTime start, QDateTime end);
    void setBuildingAndRoom(QString building, QString room);


    // Finalization Slots
    void reservationCompletedAndAcknowledged();
    void finalizeReservationForceClosed();
    void finalizerEditedReservation();

private slots:
    void on_viewItemsButton_clicked();

    void on_addItemButton_clicked();

    void on_locationButton_clicked();

    void on_timeButton_clicked();

    void on_cancelReservationButton_clicked();

    void on_completeReservationButton_clicked();

    void on_testLoadingButton_clicked();

    void reservationIsValid();
    void reservationIsInvalid(std::vector<scheduleConflict>);

private:
    Ui::CheckOut *ui;

    /*
        Differenciate checkout types
    */
    CheckoutType checkoutMode;

    ScreenBoard *screenBoard;
    QMdiSubWindow *screenSub;
    ContextManager *localContext;
    KeyboardFlow keyDirector;

    bool timeSet, itemsSet, forSet, bySet, allowView;

    Ephimeral *ephimeralReservation;

    void shutdownWindow();
    void closeEvent(QCloseEvent *event);

    void bringUpKeyboard();

    void checkForViewEnable();
};

#endif // CHECKOUT_H
