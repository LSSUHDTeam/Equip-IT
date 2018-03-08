
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
import threading
from time import sleep
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

class PathCapsule(threading.Thread):
    def __init__(self, dispatcher, port, title):
        threading.Thread.__init__(self)
        self.dispatcher = dispatcher
        self.port = port
        self.title = title
        self.httpd = make_server('', port, dispatcher)
    
    def run(self):
        self.httpd.serve_forever()

    def kill(self):
        self.httpd.shutdown()
        self.httpd.server_close()

if __name__ == '__main__':

    # Rets    
    ret_dispatcher = PathDispatcher()
    ret_dispatcher.register('GET', '/items', itemroute)
    ret_dispatcher.register('GET', '/schedules', schedroute)
    ret_dispatcher.register('GET', '/reservations', resroute)
    ret_dispatcher.register('GET', '/reminders', remroute)
    ret_dispatcher.register('GET', '/cats', catroute)
    ret_capsule = PathCapsule(ret_dispatcher, 8080, 'Get Capsule')

    # Upds
    upd_dispatcher = PathDispatcher()
    upd_dispatcher.register('GET', '/upres', updateReservation)
    upd_capsule = PathCapsule(upd_dispatcher, 9090, 'Post Capsule')

    upd_capsule.start()
    ret_capsule.start()

    try:
        while 1:
            sleep(1)
    except KeyboardInterrupt:
        print("Killing Retriever")
        ret_capsule.kill()
        ret_capsule.join()

        print("Killing Updater")
        upd_capsule.kill()
        upd_capsule.join()

