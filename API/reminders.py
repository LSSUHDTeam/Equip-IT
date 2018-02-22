


import json
import gherkindb

from equipit import generateError, generateDayStamp

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