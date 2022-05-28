import socket
from utils_1705027 import *
from SocketConnection_1705027 import SocketConnection
from aes_1705027 import AES, initAES, RunAES
from rsa_1705027 import RSA
import os

#directory
directory = "./Don't Open This"
try:
    os.mkdir(directory)
except:
    pass

#socket open
s = socket.socket()
port = 12345
s.bind(('', port))
s.listen(1)

c, addr = s.accept()
print('Got connection from', addr)

sc = SocketConnection(c)

#text , key, K
plaintext = input("Enter plain text: ")
key = input("Enter key: ")
K = int(input("Enter K: "))
# plaintext = 'Can They Do Their Fest in upcoming june'
# key = 'BUET CSE17 Batch'
# K = 64


#aes part
aes, allkeysArray = initAES(key, 128)
cipher = RunAES(aes, plaintext, "encrypt")
keyArr = aes.keyArr #key hex array

#rsa part
rsa = RSA()
rsa.keyGenerator(K) #public key and private key generation
PublicKey = [rsa.e, rsa.n]
PrivateKey = [rsa.d, rsa.n]


#saving private key in a file
privateKeyFile = "./Don't Open This/PRK.txt"
prk = open(privateKeyFile, "w")
prk.write(str(PrivateKey[0]) + " " + str(PrivateKey[1]))
prk.close()

#encrypting key using RSA
valArr = []
for val in keyArr:
    valArr.append(rsa.encrypt(int(val)))

#sending public key to receiver
sc.send(PublicKey)
(c.recv(1024).decode())

#sending encrypted aes key to receiver
sc.send(valArr)
(c.recv(1024).decode())

#sending cipher text to receiver
sc.send(cipher)
(c.recv(1024).decode())

#dpt read
plaintextFile = "./Don't Open This/DPT.txt"
pt = open(plaintextFile, "r")
dpt = pt.read()
pt.close()

print("dpt : ", dpt)

if dpt.strip() == plaintext.strip():
    print("Successful Decryption")
else:
    print("Unsuccessful Decryption")

c.close()