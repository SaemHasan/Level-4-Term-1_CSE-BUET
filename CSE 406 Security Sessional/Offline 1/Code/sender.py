import socket
from utils import *
from SocketConnection import SocketConnection
from aes_v2 import AES, initAES, RunAES
from rsa import RSA
import os

#directory
directory = "./Don't Open This"
try:
    os.mkdir(directory)
except:
    pass


s = socket.socket()
port = 12345
s.bind(('', port))
s.listen(1)

c, addr = s.accept()
print('Got connection from', addr)

sc = SocketConnection(c)

#text , key, K
plaintext = 'CanTheyDoTheirFestinupcomingjune'
key = 'BUET CSE17 Batch'
K = 64


#aes part
aes, allkeysArray = initAES(key, 128)
cipher = RunAES(aes, plaintext, "encrypt")

#rsa part
keyArr = aes.keyArr
# printArrayHex(keyArr)

rsa = RSA()
rsa.keyGenerator(K)
PublicKey = [rsa.e, rsa.n]
PrivateKey = [rsa.d, rsa.n]

#privatekey
privateKeyFile = "./Don't Open This/PRK.txt"
prk = open(privateKeyFile, "w")
prk.write(str(PrivateKey[0]) + " " + str(PrivateKey[1]))
prk.close()

valArr = []
for val in keyArr:
    valArr.append(rsa.encrypt(int(val)))

sc.send(PublicKey)
(c.recv(1024).decode())

sc.send(valArr)
(c.recv(1024).decode())

sc.send(cipher)
(c.recv(1024).decode())

#dpt read
plaintextFile = "./Don't Open This/DPT.txt"
pt = open(plaintextFile, "r")
dpt = pt.read()
pt.close()

print("dpt : ", dpt)

if dpt == plaintext:
    print("Successful Decryption")
else:
    print("Unsuccessful Decryption")

c.close()