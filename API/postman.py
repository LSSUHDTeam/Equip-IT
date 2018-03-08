import requests

token = "fefd8a1a97021dbab2d105c4784a1906cd89fc575009387d378b8807192c16e3"

def display_resp(resp):
    print('''
    Status Code         {}

    Reasoning           {}

    Response Body       {}
    
    '''.format(resp.status_code, resp.reason, resp.text))

def update_reservation():
    global token

    display_resp(
        requests.post("127.0.0.1:9090", data={'token':token})
        )


update_reservation()