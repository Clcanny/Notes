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


gkeys = []
gkeys.append("Action")
gkeys.append("Nonce")
gkeys.append("Region")
gkeys.append("SecretId")
gkeys.append("SignatureMethod")
gkeys.append("Timestamp")

gvalues = []
gvalues.append("TextTranslate")
gvalues.append(random.randint(1, 512))
gvalues.append("gz")
gvalues.append(secretId)
gvalues.append("HmacSHA256")
gvalues.append(int(time.time()))

def concat(keys, values):
    s = ""
    size = len(keys)
    for i in range(size):
        s += keys[i].replace("_", ".")
        s += "="
        s += str(values[i])
        s += "&"
    s = s[:-1]
    return s

def readySignature(keys, values):
    return "GETtmt.api.qcloud.com/v2/index.php?" + concat(keys, values)

def signature(keys, values):
    digest = hmac.new(bytes(secretKey, "utf-8"), msg=bytes(readySignature(keys, values), "utf-8"), digestmod=hashlib.sha256).digest()
    signature = base64.b64encode(digest)
    return signature

def translate(source, target, sourceText):
    keys = gkeys[:]
    keys.append("source")
    keys.append("sourceText")
    keys.append("target")
    assert(keys[1] == "Nonce")
    assert(keys[5] == "Timestamp")

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
    payload["Signature"] = signature(keys, values)

    r = requests.get('https://tmt.api.qcloud.com/v2/index.php', params = payload)
    return r.text

r = translate("zh", "en", "你好")
print(r)
r = translate("en", "zh", "Hello, my babe!")
print(r)