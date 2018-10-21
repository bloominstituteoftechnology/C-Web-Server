import requests

for i in range(4):
    requests.post('http://localhost:3490/save', data={i: i}, headers={})

