import os
import gherkindb
from equipit import generateTimeSimestamp, generateDayStamp

from hashlib import sha256

def addToSchedule(item_barcode, res_id, start, end):

    sid = sha256(
        ( res_id + item_barcode + start + end ).encode()
        ).hexdigest()

    db = gherkindb.load("databases/schedules.db", True)

    # Items make their own schedule lists, but just in-case
    # we do a None check
    if db.get(item_barcode) is None:
        db.lcreate(item_barcode)

    db.ladd(item_barcode, {
        "sid": sid,
        "resid": res_id,
        "start": start,
        "end": end
    })
    return sid

def addItem(item_dict):
    if not isinstance(item_dict, dict):
        return -1
    db = gherkindb.load("databases/items.db", True)
    check = db.get(item_dict["barcode"])
    if check is not None:
        return -2

    # Add the item
    db.set(item_dict["barcode"], item_dict)

    # Create a schedule list for the item
    dbs = gherkindb.load("databases/schedules.db", True)
    if dbs.get(item_dict["barcode"]) is None:
        dbs.lcreate(item_dict["barcode"])
    return 0

def addReservation(res_dict):
    if not isinstance(res_dict, dict):
        return -1
    dbti = gherkindb.load("databases/res_ti.db", True)
    dbid = gherkindb.load("databases/res_id.db", True)
    dbresid = gherkindb.load("databases/uniqueids.db", True)

    # Reservation already exists - This isn't an edit function
    if dbid.get(res_dict["id"]) is not None:
        return -2

    # Obtain a new reservation id
    new_res_id = dbresid.get("next_res_id")
    if new_res_id is None:
        new_res_id = 0
    dbresid.set("next_res_id", new_res_id+1)
    new_res_id = str(new_res_id)

    res_dict["id"] = new_res_id

    res_dict["created"] = generateTimeSimestamp()

    # Add to the comprehensive db
    dbid.set(res_dict["id"], res_dict)

    # Time-indexed db will have multiple (potentially) per-day
    # If it doesn't exist, we need to make a list
    if dbti.get(res_dict["ti"]) is None:
        dbti.lcreate(res_dict["ti"])
    
    # Add the teservation to the time-indexed db
    dbti.ladd(res_dict["ti"], res_dict)

    # Schedule the device to be out for the time
    for item in res_dict["itemBarcodes"]:
        addToSchedule(item, res_dict["id"], res_dict["start"], res_dict["end"])

    return 0

def addReminder(rem_dict):
    if not isinstance(rem_dict, dict):
        return -1
    dbti = gherkindb.load("databases/rem_ti.db", True)
    dbid = gherkindb.load("databases/rem_id.db", True)

    # Reservation already exists - This isn't an edit function
    if dbid.get(rem_dict["id"]) is not None:
        return -2

    rem_dict["created"] = generateTimeSimestamp()

    # Add to the comprehensive db
    dbid.set(rem_dict["id"], rem_dict)

    # Time-indexed db will have multiple (potentially) per-day
    # If it doesn't exist, we need to make a list
    if dbti.get(rem_dict["ti"]) is None:
        dbti.lcreate(rem_dict["ti"])
    
    # Add the teservation to the time-indexed db
    dbti.ladd(rem_dict["ti"], rem_dict)
    return 0

def addCategory(cat_dict):
    if not isinstance(cat_dict, dict):
        return -1
    db = gherkindb.load("databases/cats.db", True)
    dbresid = gherkindb.load("databases/catids.db", True)

    # Reservation already exists - This isn't an edit function
    if db.get(cat_dict["id"]) is not None:
        return -2

    # Force unique labels
    for catkey in db.scpyall():
        cat = db.get(catkey)
        if cat["label"] == cat_dict["label"]:
            return -3

    # Obtain a new reservation id
    new_res_id = dbresid.get("next_res_id")
    if new_res_id is None:
        new_res_id = 0
    dbresid.set("next_res_id", new_res_id+1)
    new_res_id = str(new_res_id)

    cat_dict["id"] = new_res_id

    # Add to the db
    db.set(cat_dict["id"], cat_dict)
    return 0

'''
    Load the dbs with some test info
'''
if __name__ == '__main__':

    db = gherkindb.load("databases/config.db", True)
    if db.get("qtApiToken") is None:
        print("Loading development api token")
        db.set("qtApiToken", 
        "fefd8a1a97021dbab2d105c4784a1906cd89fc575009387d378b8807192c16e3")

    print("Adding item: ", addItem({
        "barcode": "938-x837-3284",
        "name": "Projector 1",
        "desc": "A test item named projector",
        "periphs": [
            {
                "name": "VGA Cable",
                "desc": "A VGA cable to hook-up to a computer",
                "count": 2,
                "numberpresent": 2
            },
            {
                "name": "Remote",
                "desc": "A remote for controlling the projector",
                "count": 1,
                "numberpresent": 1
            }
        ]
    }))

    print("Adding item: ", addItem({
        "barcode": "929-x837-3284",
        "name": "Projector 2",
        "desc": "A test item named projector",
        "periphs": [
            {
                "name": "VGA Cable",
                "desc": "A VGA cable to hook-up to a computer",
                "count": 2,
                "numberpresent": 2
            },
            {
                "name": "Remote",
                "desc": "A remote for controlling the projector",
                "count": 1,
                "numberpresent": 1
            }
        ]
    }))

    print("Adding cat: ", addCategory({
        "id": "GIVEN BY CAT ADDER",
        "label": "Projectors",
        "parent": "NONE",
        "itembarcodes": [
            "929-x837-3284",
            "938-x837-3284"
        ]
    }))

    # print("Adding reservation: ", addReservation({
    #     "id": "GIVEN TO BY ADDRES",
    #     "ti": generateDayStamp(),
    #     "title": "A test reservation for testing - that will cause conflict!",
    #     "created": "CHANGED WHEN ADDED TO DB",
    #     "wfor": "Bosley",
    #     "by": "A00167484",
    #     "start": "08/03/2018 9:30 AM",
    #     "end": "08/03/2018 7:30 PM",
    #     "status": "pending",
    #     "retby": "na",
    #     "email": "na",
    #     "itemBarcodes": [
    #         "938-x837-3284",
    #         "929-x837-3284"
    #     ]
    # }))

    # print("Adding reservation: ", addReservation({
    #     "id": "GIVEN TO BY ADDRES",
    #     "ti": generateDayStamp(),
    #     "title": "A test reservation for testing!",
    #     "created": "CHANGED WHEN ADDED TO DB",
    #     "wfor": "Bosley",
    #     "by": "A00167484",
    #     "start": "12/03/2018 5:30 PM",
    #     "end": "13/03/2018 6:30 PM",
    #     "status": "out",
    #     "retby": "na",
    #     "email": "na",
    #     "itemBarcodes": [
    #         "929-x837-3284"
    #     ]
    # }))

    print("Adding reminder: ", addReminder({
        "id": "0000",
        "ti": generateDayStamp(),
        "title": "Feed the fish!",
        "created": "CHANGED WHEN ADDED TO DB",
        "desc": "Some lengthy description about how to feed the fish and who to call if help is needed",
        "start": "10:30. Wed March 5 18",
        "end": "14:30. Wed March 7 18",
        "by": "A00167484",
        "status": "incomplete"
    }))