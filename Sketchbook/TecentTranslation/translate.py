# -*- coding: utf-8 -*-

import requests
import time
import random
import sys

import hmac
import hashlib
import base64

secretId = 'your secret id'
secretKey = 'your secret key'

gkeys = []
gkeys.append('Action')
gkeys.append('Nonce')
gkeys.append('Region')
gkeys.append('SecretId')
gkeys.append('SignatureMethod')
gkeys.append('Timestamp')

gvalues = []
gvalues.append('TextTranslate')
gvalues.append(None)
gvalues.append('gz')
gvalues.append(secretId)
gvalues.append('HmacSHA256')
gvalues.append(None)

def concat(keys, values):
    s = ''
    size = len(keys)
    for i in range(size):
        s += keys[i].replace('_', '.')
        s += '='
        s += str(values[i])
        s += '&'
    s = s[:-1]
    return s

def readySignature(keys, values):
    return 'GETtmt.api.qcloud.com/v2/index.php?' + concat(keys, values)

def signature(keys, values):
    if sys.version_info[0] < 3:
        digest = hmac.new(bytes(secretKey), msg=bytes(readySignature(keys, values)), digestmod=hashlib.sha256).digest()
    else:
        digest = hmac.new(bytes(secretKey, 'utf-8'), msg=bytes(readySignature(keys, values), 'utf-8'), digestmod=hashlib.sha256).digest()
    signature = base64.b64encode(digest)
    return signature

def translate(source, target, sourceText):
    keys = gkeys[:]
    keys.append('source')
    keys.append('sourceText')
    keys.append('target')
    assert(keys[1] == 'Nonce')
    assert(keys[5] == 'Timestamp')

    values = gvalues[:]
    values.append(source)
    values.append(sourceText)
    values.append(target)
    values[1] = random.randint(1, 512)
    values[5] = int(time.time())

    payload = {}
    size = len(keys)
    for i in range(size):
        payload[keys[i]] = values[i]
    payload['Signature'] = signature(keys, values)

    r = requests.get('https://tmt.api.qcloud.com/v2/index.php', params = payload)
    return r.text

r = translate('zh', 'en', '你好')
print(r)
r = translate('en', 'zh', 'Hello, my babe!')
print(r)
