#ifndef WINDOWDESCRIPTORS_H
#define WINDOWDESCRIPTORS_H

#include <QString>

enum class WindowDescriptors{

    LoginWindow,
    BaseWindow,
    ReservationRoot,
    ReservationAdd,
    ReservationRemove,
    CheckoutRoot,
    CheckInRoot,
    OverviewRoot,
    AdminPanel,
    _non_window_DAM,
};

class WindowHelper
{
public:
    WindowHelper(){}
    ~WindowHelper(){}

    QString getStringFromWid(WindowDescriptors wid)
    {
        switch(wid){
        case WindowDescriptors::LoginWindow:
            return "LoginWindow";
        case WindowDescriptors::BaseWindow:
            return "BaseWindow";
        case WindowDescriptors::ReservationRoot:
            return "ReservationRoot";
        case WindowDescriptors::ReservationAdd:
            return "ReservationAdd";
        case WindowDescriptors::ReservationRemove:
            return "ReservationRemove";
        case WindowDescriptors::CheckoutRoot:
            return "CheckoutRoot";
        case WindowDescriptors::CheckInRoot:
            return "CheckInRoot";
        case WindowDescriptors::OverviewRoot:
            return "OverviewRoot";
        case WindowDescriptors::AdminPanel:
            return "AdminPanel";
        case WindowDescriptors::_non_window_DAM:
            return "Data Access Manager (framework)";
        }
        return "Not Found";
    }
};

#endif // WINDOWDESCRIPTORS_H
