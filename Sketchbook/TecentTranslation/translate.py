import requests
import time
import random

import hmac
import hashlib
import base64

secretId = "AKID8AZptlOzNbdU8bpVijR1PbbS73sVnMWU"
secretKey = "3vb5FI9oHYgdP0PbbP2Yu8B2g7MUQI4g"

global_payload = {}
global_payload["Action"] = "TextTranslate"
global_payload["Nonce"] = 3253
global_payload["Region"] = "gz"
global_payload["SecretId"] = secretId
global_payload["SignatureMethod"] = "HmacSHA256"
global_payload["Timestamp"] = int(time.time())

def concat(dictionary):
    s = ""
    for key, value in dictionary.items():
        s += key.replace("_", ".")
        s += "="
        s += str(value)
        s += "&"
    s = s[:-1]
    return s

def readySignature(dictionary):
    dictionary["Timestamp"] = int(time.time())
    dictionary["Nonce"] = random.randint(1, 512)
    return "GETtmt.api.qcloud.com/v2/index.php?" + concat(dictionary)

def signature(dictionary):
    digest = hmac.new(bytes(secretKey, "utf-8"), msg=bytes(readySignature(dictionary), "utf-8"), digestmod=hashlib.sha256).digest()
    signature = base64.b64encode(digest)
    return signature

def translate(source, target, sourceText):
    payload = dict(global_payload)
    payload["source"] = source
    payload["sourceText"] = sourceText
    payload["target"] = target
    payload["Signature"] = signature(payload)
    r = requests.get('https://tmt.api.qcloud.com/v2/index.php', params = payload)
    return r

r = translate("zh", "en", "你好")
print(r.text)
r = translate("en", "zh", "Hello, my babe!")
print(r.text)
