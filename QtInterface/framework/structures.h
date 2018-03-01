#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QString>
#include <vector>
#include <QStringList>

// Small things that get coupled with reservables, but aren't kept track of
struct peripherals {
    QString name, desc;
    int count, numberpresent;

    peripherals& operator=(const peripherals &other)
    {
        name = other.name; desc = other.desc; count = other.count;
        numberpresent = other.numberpresent;
        return *this;
    }

    bool operator==(const peripherals &other)
    {
        return (name == other.name);
    }
};

// Valuable things that are reserverd
struct reservableItems {
    // Schedule id ties to schedule item to track usability
    QString barcode, name, desc;
    std::vector<peripherals> periphs;

    reservableItems& operator=(const reservableItems &other)
    {
        barcode = other.barcode; name = other.name; desc = other.desc;
        periphs = other.periphs;
        return *this;
    }
};

// Item schedule info
struct scheduleEntry{
    QString sid, start, end, resid;
};

// The item's schedule
struct schedule{
    QString scheduleid; // Same as an item's barcode
    std::vector<scheduleEntry> scheduleInformation;

};

// Reservation structure
struct reservations {
    QString id, ti, title, created, wfor, by, start, end, status;
    QStringList itemBarcodes;
};

// Reminders!
struct reservedReminders {
    QString id, ti,title, created, desc, start, end, by, status;
};

// Categories
struct itemCategories {
    QString id, label, parent;
    QStringList barcodes;
};

#endif // STRUCTURES_H
