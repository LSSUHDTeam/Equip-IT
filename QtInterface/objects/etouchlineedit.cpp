#include "etouchlineedit.h"


void ETouchLineEdit::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    emit focusGained();
}

void ETouchLineEdit::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    emit focusLoss();
}
