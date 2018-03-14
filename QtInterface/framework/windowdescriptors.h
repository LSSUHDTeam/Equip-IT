#ifndef WINDOWDESCRIPTORS_H
#define WINDOWDESCRIPTORS_H

#include <QString>

enum class WindowDescriptors{

    LoginWindow,
    BaseWindow,
    ReservationRoot,
    ReservationEdit,
    CheckoutRoot,
    CheckInRoot,
    OverviewRoot,
    AdminPanel,
    _non_window_DAM,
    _non_window_ephimeral,
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
        case WindowDescriptors::ReservationEdit:
            return "ReservationEdit";
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
        case WindowDescriptors::_non_window_ephimeral:
            return "Ephimeral reservation (framework)";
        }
        return "Not Found";
    }
};

#endif // WINDOWDESCRIPTORS_H
