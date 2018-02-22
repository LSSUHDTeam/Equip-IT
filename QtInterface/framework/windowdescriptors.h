#ifndef WINDOWDESCRIPTORS_H
#define WINDOWDESCRIPTORS_H

#include <QString>

enum class WindowDescriptors{

    LoginWindow,
    BaseWindow,
    ReservationRoot,
    ReservationAdd,
    ReservationRemove,
    InstantCheckoutRoot,
    OverviewRoot,

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
        case WindowDescriptors::InstantCheckoutRoot:
            return "InstantCheckoutRoot";
        case WindowDescriptors::OverviewRoot:
            return "OverviewRoot";
        }
        return "Not Found";
    }
};

#endif // WINDOWDESCRIPTORS_H
