#!/usr/bin/python
#coding=utf-8

import datetime, time
from flask import *


app = Flask(__name__)

@app.route('/')
def server_time():
    return jsonify([29001, time.strftime('%Y.%m.%d %H:%M:%S',time.localtime(time.time()))])

if __name__ == '__main__':
    app.run(host='0.0.0.0',port=29001,debug=True)
