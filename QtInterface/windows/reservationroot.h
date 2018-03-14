#ifndef RESERVATIONROOT_H
#define RESERVATIONROOT_H

#include <QDebug>
#include <QMainWindow>
#include <QCloseEvent>
#include "windows/checkout.h"
#include "framework/contextmanger.h"
#include "framework/windowdescriptors.h"

namespace Ui {
class ReservationRoot;
}

class ReservationRoot : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReservationRoot(ContextManager * context, QWidget *parent = 0);
    ~ReservationRoot();

signals:
    void forceCloseChildren();
    void reservationRootClosed(WindowDescriptors window);

public slots:
    void forceClose();

    void childWindowClosed(WindowDescriptors);

private slots:
    void on_newResButton_clicked();

    void on_editExistingButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ReservationRoot *ui;
    bool controlAvailability;
    ContextManager *localContext;

    void shutdownWindow();

    void closeEvent(QCloseEvent *event);

    void toggleControlAvailabilty();
};

#endif // RESERVATIONROOT_H
