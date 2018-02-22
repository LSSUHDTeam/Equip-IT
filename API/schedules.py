


import json
import gherkindb

from equipit import generateError
def getSchedules():
    db = gherkindb.load("databases/schedules.db", True)
    schedules = []
    for key in db.scpyall():
        sched = db.get(key)
        if sched is None:
            return generateError("DB Error", "No, or 'None' item key(s)")
        schedules.append(sched)
    if len(schedules) == 0:
        return generateError("DB Error", "There are no schedules found in databases/res_id.db")
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
        resp = getAllItems()
    else:
        resp = generateError("Parameter Error", "Invalid classifier in /schedule")
    try:
        resp = resp.encode('utf-8')
    except AttributeError:
        resp = generateError("Internal Error", "Tried to encode a non-encodable object.")
    yield resp