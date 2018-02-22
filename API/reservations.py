
"""

    res_ti.db   -> Time-indexed db
    res_id.db   -> id-indexed db

"""

import json
import gherkindb

from equipit import generateError, generateDayStamp

def getTodaysReservations():
    db = gherkindb.load("databases/res_ti.db", True)
    todays_list = db.get(generateDayStamp())
    if todays_list is None or len(todays_list) == 0:
        return json.dumps([])
    reservations = []
    for reservation in todays_list:
        reservations.append(reservation)
    try:
        reservations = json.dumps(reservations)
    except:
        return generateError("DB Error", "Json unable to dump reservations list to string")
    return reservations

def getAllReservations():
    db = gherkindb.load("databases/res_id.db", True)
    reservations = []
    for key in db.scpyall():
        res = db.get(key)
        if res is None:
            return generateError("DB Error", "No, or 'None' item key(s)")
        reservations.append(res)
    if len(reservations) == 0:
        return generateError("DB Error", "There are no reservations found in databases/res_id.db")
    try:
        reservations = json.dumps(reservations)
    except:
        return generateError("DB Error", "Json unable to dump reservations list to string")
    return reservations

''' Handles /reservation calls '''
def resroute(environ, start_response):
    start_response('200 OK', [('Content-type', 'text/plain')])
    params = environ['params']
    frame = params.get('frame')
    if frame == "today":
        resp = getTodaysReservations()
    elif frame == "all":
        resp = getAllReservations()
    else:
        resp = generateError("Parameter Error", "Invalid frame in /reservations")
    try:
        resp = resp.encode('utf-8')
    except AttributeError:
        resp = generateError("Internal Error", "Tried to encode a non-encodable object.")
    yield resp