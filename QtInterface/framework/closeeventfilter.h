#ifndef CLOSEEVENTFILTER_H
#define CLOSEEVENTFILTER_H

#include <QObject>
#include <QEvent>
#include "framework/windowdescriptors.h"

class CloseEventFilter : public QObject
{
    Q_OBJECT
public:
    CloseEventFilter(WindowDescriptors wid, QObject *parent) : QObject(parent) {
        this->wid = wid;
    }

signals:
    void closedTriggered(WindowDescriptors);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    WindowDescriptors wid;
};

#endif // CLOSEEVENTFILTER_H
