#include "closeeventfilter.h"

bool CloseEventFilter::eventFilter(QObject *obj, QEvent *event)
{
     if (event->type() == QEvent::Close)
     {
          emit closedTriggered(this->wid);
     }
     return QObject::eventFilter(obj, event);
}
