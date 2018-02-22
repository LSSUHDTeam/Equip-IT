
import json
import gherkindb

from datetime import datetime

'''
    Generators
'''
def generateError(err, message):
    return '[{"error": "%s", "message": "%s"}]' % (err, message)

def generateTimeSimestamp():
    return datetime.now().strftime("%d-%m-%Y:%H-%M-%S")

def generateDayStamp():
    return datetime.now().strftime("%d-%Y")

'''
    GET
    Reminders

'''
def getTodaysReminders():
    db = gherkindb.load("databases/rem_ti.db", True)
    todays_list = db.get(generateDayStamp())
    if todays_list is None or len(todays_list) == 0:
        return json.dumps([])
    reminders = []
    for reminder in todays_list:
        reminders.append(reminder)
    try:
        reminders = json.dumps(reminders)
    except:
        return generateError("DB Error", "Json unable to dump reminders list to string")
    return reminders

def getAllReminders():
    db = gherkindb.load("databases/rem_id.db", True)
    reminders = []
    for key in db.scpyall():
        rem = db.get(key)
        if rem is None:
            return generateError("DB Error", "No, or 'None' item key(s)")
        reminders.append(rem)
    if len(reminders) == 0:
        return generateError("DB Error", "There are no reminders found in databases/res_id.db")
    try:
        reminders = json.dumps(reminders)
    except:
        return generateError("DB Error", "Json unable to dump reminders list to string")
    return reminders

''' Handles /reminders calls '''
def remroute(environ, start_response):
    start_response('200 OK', [('Content-type', 'text/plain')])
    params = environ['params']
    frame = params.get('frame')

    if frame == "today":
        resp = getTodaysReminders()
    elif frame == "all":
        resp = getAllReminders()
    else:
        resp = generateError("Parameter Error", "Invalid frame in /reminders")
    try:
        resp = resp.encode('utf-8')
    except AttributeError:
        resp = generateError("Internal Error", "Tried to encode a non-encodable object.")
    yield resp

'''
    GET
    Items

'''
def getAllItems():
    db = gherkindb.load("databases/items.db", True)
    items = []
    for key in db.scpyall():
        item = db.get(key)
        if item is None:
            return generateError("DB Error", "No, or 'None' item key(s)")
        items.append(item)
    if len(items) == 0:
        return generateError("DB Error", "There are no items found in databases/items.db")
    try:
        items = json.dumps(items)
    except:
        return generateError("DB Error", "Json unable to dump item list to string")
    return items

''' Handles /item calls '''
def itemroute(environ, start_response):
    start_response('200 OK', [('Content-type', 'text/plain')])
    params = environ['params']
    classifier = params.get('classifier')
    if classifier == "all":
        resp = getAllItems()
    else:
        resp = generateError("Parameter Error", "Invalid classifier in /items")
    try:
        resp = resp.encode('utf-8')
    except AttributeError:
        resp = generateError("Internal Error", "Tried to encode a non-encodable object.")
    yield resp

'''
    GET
    Schedules

'''
def getSchedules():
    db = gherkindb.load("databases/schedules.db", True)
    schedules = []
    for key in db.scpyall():
        sched = db.get(key)
        if sched is None:
            return generateError("DB Error", "No, or 'None' item key(s)")
        schedules.append({
            "id": key,
            "schedule": sched
        })
    if len(schedules) == 0:
        return generateError("DB Error", "There are no schedules found in databases/schedules.db")
    try:
        schedules = json.dumps(schedules)
    except:
        return generateError("DB Error", "Json unable to dump schedules list to string")
    return schedules


''' Handles /schedule calls '''
def schedroute(environ, start_response):
    start_response('200 OK', [('Content-type', 'text/plain')])
    params = environ['params']
    classifier = params.get('classifier')
    if classifier == "all":
        resp = getSchedules()
    else:
        resp = generateError("Parameter Error", "Invalid classifier in /schedule")
    try:
        resp = resp.encode('utf-8')
    except AttributeError:
        resp = generateError("Internal Error", "Tried to encode a non-encodable object.")
    yield resp

'''
    GET
    Reservations

'''
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