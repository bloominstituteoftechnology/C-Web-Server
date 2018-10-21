import requests

for i in range(4):
    res = requests.post('http://localhost:3490/save', data={i: i}, headers={})
    print(res.text)

