
"""
    /items
        ?classifier=all

    /schedules
        ?classifier=all

    /reservations
        ?frame=today
        ?frame=all
        ?frame=range&start=date&end=date        NYD

    /reminders
        ?frame=today
        ?frame=all
"""

__author__ = 'Josh Allen Bosley'

import cgi
from wsgiref.simple_server import make_server

from equipit import *

def notfound_404(environ, start_response):
    start_response('404 Not Found', [('Content-type', 'text/plain')])
    return [generateError("[404] Request error", "This is not the page you're looking for.").encode()]

class PathDispatcher:
    def __init__(self):
        self.pathmap = {}

    def __call__(self, environ, start_response):
        path = environ['PATH_INFO']
        params = cgi.FieldStorage(environ['wsgi.input'],
                                  environ=environ)
        method = environ['REQUEST_METHOD'].lower()
        environ['params'] = { key: params.getvalue(key) for key in params }
        handler = self.pathmap.get((method,path), notfound_404)
        return handler(environ, start_response)

    def register(self, method, path, function):
        self.pathmap[method.lower(), path]= function
        return function

if __name__ == '__main__':

    dispatcher = PathDispatcher()

    # Gets    
    dispatcher.register('GET', '/items', itemroute)
    dispatcher.register('GET', '/schedules', schedroute)
    dispatcher.register('GET', '/reservations', resroute)
    dispatcher.register('GET', '/reminders', remroute)

    # Posts

    httpd = make_server('', 8080, dispatcher)
    print('Serving on port 8080')

    try:
        httpd.serve_forever()
    except:
        print("Shutting down")
        exit(1)
