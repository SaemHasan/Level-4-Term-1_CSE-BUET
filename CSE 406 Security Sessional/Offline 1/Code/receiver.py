import socket
from SocketConnection import SocketConnection
from aes_v2 import *
from rsa import *

#socket part
s = socket.socket()
port = 12345
s.connect(('127.0.0.1', port))
rc = SocketConnection(s)

#public key receive
publicKey = rc.recieve()
s.send("Public key received".encode())


#private Key read from file
privateKeyFile = "./Don't Open This/PRK.txt"
prk = open(privateKeyFile, "r")
pk = prk.read()
prk.close()
prk = pk.split(" ")
privateKey = [int(prk[0]), int(prk[1])]



#rsa setup
rsa = RSA()
rsa.setKeys(publicKey[1], publicKey[0], privateKey[0])

#key receive
valArr = rc.recieve()
keyArr = []
for val in valArr:
    keyArr.append(int(rsa.decrypt(int(val))))

KEY = getStringFromHexArray(keyArr)
print("key: ", KEY)
s.send("key received".encode())

#aes setup
aes, allkeysArray = initAES(KEY, 128)

#cipher text receive
cipher = rc.recieve()

plain = RunAES(aes, cipher, "decrypt")

plain = getPlainText(plain)
print("plaintext: ", plain)


#plaint text write
plaintextFile = "./Don't Open This/DPT.txt"
pt = open(plaintextFile, "w")
pt.write(plain)
pt.close()

s.send("plaintext received".encode())

#connection close
s.close()