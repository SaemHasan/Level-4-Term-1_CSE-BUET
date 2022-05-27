from utils import *


class AES:
    def __init__(self, roundLength, keyLength):
        self.roundLength = roundLength + 1
        self.keyLength = keyLength
        self.wordCount = self.keyLength//32
        self.arr = []  # all keys
        return

    def stringtoHex(self, hexstring):
        hexstring = hexstring.encode('utf-8')
        # print(hexstring)
        return hexstring.hex()

    def readFromSbox(self, idx):
        # print(str(hex(idx))[2:])
        b = BitVector(hexstring=str(hex(idx))[2:])
        int_val = b.intValue()
        s = Sbox[int_val]
        s = BitVector(intVal=s, size=8)
        return s.get_bitvector_in_hex()

    def g(self, word, rnd=0):
        ar = np.array(word)
        ar = np.roll(ar, -1)
        ar = [self.readFromSbox(x) for x in ar]
        ar = [int(x, 16) for x in ar]
        # print("here in g")
        # print(ar)
        roundConst = [rc[rnd], 00, 00, 00]
        ar = self.list_xor(ar, roundConst)
        return ar

    def list_xor(self, a, b):
        return [x ^ y for x, y in zip(a, b)]

    def keyGenerator(self, key):
        key = self.stringtoHex(key)
        keyArray = [key[x] + key[x + 1] for x in range(0, len(key), 2)]
        keyArray = [int(x, 16) for x in keyArray]
        # print(keyArray)
        # print(self.g(keyArray[:4]))

        for i in range(0, self.roundLength):
            c = []
            for j in range(0, self.wordCount):
                if i != 0:
                    if j == 0:
                        w = self.arr[i - 1][j]
                        gval = self.g(self.arr[i - 1][j+3], i-1)
                        c.append(self.list_xor(w, gval))
                    else:
                        w = self.arr[i-1][j]
                        gval = c[j-1]
                        c.append(self.list_xor(w, gval))
                else:
                    c.append(keyArray[0+j*4:4+j*4])
            self.arr.append(c)
        return self.arr

    def encrypt(self, plainText, key):
        plainTextHex = self.stringtoHex(plainText)
        keyHex = self.stringtoHex(key)
        # print(keyHex)
        keyGen = self.keyGenerator(key)
        print(keyGen[0])
        for x in keyGen:
            for y in x:
                y = [hex(h) for h in y]
                print(y)
            print("\n")


rc = [1, 2, 4, 8, 16, 32, 64, 128, 27, 54]
keys = [128, 192, 256]
rounds = [10, 12, 14]
aes = AES(rounds[0], keys[0])
plaintext = 'CanTheyDoTheirFest'
key = 'Thats my Kung Fu'
aes.encrypt(plaintext, key)
