#ifndef CHECKOUT_H
#define CHECKOUT_H

#include <QDebug>
#include <QMdiArea>
#include <QMainWindow>
#include <QCloseEvent>
#include "framework/contextmanger.h"
#include "windows/screenboard.h"
#include "windows/quickscanitem.h"
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
    void forceClose();

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

private slots:
    void on_viewItemsButton_clicked();

    void on_addItemButton_clicked();

    void on_locationButton_clicked();

    void on_timeButton_clicked();

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

    void shutdownWindow();
    void closeEvent(QCloseEvent *event);

    void bringUpKeyboard();
};

#endif // CHECKOUT_H
