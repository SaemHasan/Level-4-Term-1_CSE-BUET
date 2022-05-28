from pandas import array
from utils import *


class AES:
    AES_modulus = BitVector(bitstring='100011011')

    def __init__(self, roundLength, keyLength):
        self.roundLength = roundLength + 1
        self.keyLength = keyLength
        self.wordCount = self.keyLength//32
        self.arr = np.array([])  # all keys
        self.keyArr = []
        return

    def stringtoHex(self, hexstring):
        hexstring = hexstring.encode('utf-8')
        # print(hexstring)
        return hexstring.hex()

    def readFromSbox(self, idx):
        # print(idx, str(hex(idx))[2:])
        b = BitVector(hexstring=str(hex(idx))[2:])
        int_val = b.intValue()
        s = Sbox[int_val]
        s = BitVector(intVal=s, size=8)
        return s.int_val()

    def readFromInvSbox(self, idx):
        # print(idx, str(hex(idx))[2:])
        b = BitVector(hexstring=str(hex(idx))[2:])
        int_val = b.intValue()
        s = InvSbox[int_val]
        s = BitVector(intVal=s, size=8)
        return s.int_val()

    def g(self, word, rnd=0):
        ar = np.array(word)
        ar = np.roll(ar, -1)  # circular left shift
        ar = [self.readFromSbox(x) for x in ar]
        roundConst = np.array([rc[rnd], 0, 0, 0])
        ar = np.bitwise_xor(ar, roundConst)
        # print("Here in G method")
        # print(ar)
        return ar
    
    def handleKey(self, key):
        keylen = self.keyLength//8
        length = len(key)
        if length == keylen:
            self.keyGenerator(key)
        elif length < keylen:
            self.keyGenerator(key + '0'*(keylen-length))
        elif length > keylen:
            self.keyGenerator(key[:keylen])

    def keyGenerator(self, key):
        key = self.stringtoHex(key)

        keyArray = np.array([int(key[x:x+2], 16)
                            for x in range(0, len(key), 2)])
        self.keyArr = keyArray
        # printArrayHex(keyArray)
        a = []
        for i in range(0, self.roundLength):
            c = []
            for j in range(0, self.wordCount):
                if i != 0:
                    if j == 0:
                        w = a[i - 1][j]
                        gval = self.g(a[i - 1][j+3], i-1)
                        c.append(np.bitwise_xor(w, gval))
                    else:
                        w = a[i-1][j]
                        gval = c[j-1]
                        c.append(np.bitwise_xor(w, gval))
                else:
                    c.append(keyArray[0+j*4:4+j*4])
            c = np.array(c)
            # print2DArrayHex(c)
            a.append(c)
        self.arr = np.array(a)
        # print3DArrayHex(self.arr)
        return self.arr

    def shiftrows(self, array):
        array[1] = np.roll(array[1], -1)
        array[2] = np.roll(array[2], -2)
        array[3] = np.roll(array[3], -3)
        # print(array)
        return array

    def invshiftrows(self, array):
        array[1] = np.roll(array[1], 1)
        array[2] = np.roll(array[2], 2)
        array[3] = np.roll(array[3], 3)
        return array

    def invsubBytes(self, array):
        b = np.array(array)
        # print(b.shape, b.dtype)
        for i in range(len(array)):
            for j in range(len(array[i])):
                b[i][j] = int(self.readFromInvSbox(array[i][j]))
        return b

    def subBytes(self, array):
        b = np.array(array)
        # print(b.shape, b.dtype)
        for i in range(len(array)):
            for j in range(len(array[i])):
                b[i][j] = int(self.readFromSbox(array[i][j]))
        return b

    def invMixColumn(self, array):
        newArray = np.array(array)  # check later
        for i in range(len(InvMixer)):
            for j in range(len(array[0])):
                newArray[i][j] = 0
                for k in range(len(array)):
                    mul = InvMixer[i][k].gf_multiply_modular(
                        BitVector(intVal=array[k][j]), self.AES_modulus, 8)
                    newArray[i][j] = np.bitwise_xor(
                        newArray[i][j], mul.int_val())
        return newArray

    def mixColumn(self, array):
        newArray = np.array(array)  # check later
        for i in range(len(Mixer)):
            for j in range(len(array[0])):
                newArray[i][j] = 0
                for k in range(len(array)):
                    mul = Mixer[i][k].gf_multiply_modular(
                        BitVector(intVal=array[k][j]), self.AES_modulus, 8)
                    newArray[i][j] = np.bitwise_xor(
                        newArray[i][j], mul.int_val())
        return newArray
    
    def encryption(self, plaintext):
        #add padding
        while len(plaintext) % 16 != 0:
            plaintext += ' '
        
        ciphertexts = []
        while len(plaintext) != 0:
            pt = plaintext[:16]
            plaintext = plaintext[16:]
            ct = self.encrypt(pt)
            ciphertexts.append(ct)

        #printing ciphertexts
        # for ct in ciphertexts:
        #     printArrayHex(array2Dto1D(ct))
        return ciphertexts

    def encrypt(self, plaintext):
        # print("plaintext len : ", len(plaintext))
        plaintextHex = self.stringtoHex(plaintext)
        plaintextArray = np.array([int(plaintextHex[x:x+2], 16)
                                   for x in range(0, len(plaintextHex), 2)])
        plainTextArray2D = array1Dto2D(plaintextArray)
        roundKey = self.arr[0].transpose()
        # print(plainTextArray2D.shape, roundKey.shape)
        cipherTextArray2D = np.bitwise_xor(plainTextArray2D, roundKey)

        # round 1-9
        for i in range(1, self.roundLength-1):
            subBytes = self.subBytes(cipherTextArray2D)
            shiftrows = self.shiftrows(subBytes)
            mixColumn2D = self.mixColumn(shiftrows)
            roundKey = self.arr[i].transpose()
            cipherTextArray2D = np.bitwise_xor(mixColumn2D, roundKey)
            # print("cipher text  :  ", i)
            # print2DArrayHex(cipherTextArray2D)

        # last round
        subBytes = self.subBytes(cipherTextArray2D)
        shiftrows = self.shiftrows(subBytes)
        roundKey = self.arr[self.roundLength-1].transpose()
        cipherTextArray2D = np.bitwise_xor(shiftrows, roundKey)
        # print("cipher text  round :  ", self.roundLength-1)
        # print2DArrayHex(cipherTextArray2D)
        return cipherTextArray2D
    
    def decryption(self, ciphertexts):
        pts = []
        for i in range(len(ciphertexts)):
            ct = ciphertexts[i]
            pt = self.decrypt(ct)
            pts.append(pt)

        #printing decrypted text
        # for pt in pts:
        #     printString(array2Dto1D(pt))
        # print()

        return pts

    def decrypt(self, ciphertext):
        roundKey = self.arr[self.roundLength-1].transpose()
        plainTextArray2D = np.bitwise_xor(ciphertext, roundKey)
        # print2DArrayHex(plainTextArray2D)

        # round 9 to 1
        for i in range(self.roundLength-2, 0, -1):
            invshiftrows = self.invshiftrows(plainTextArray2D)
            invsubBytes = self.invsubBytes(invshiftrows)
            roundKey = self.arr[i].transpose()
            plainTextArray2D = np.bitwise_xor(invsubBytes, roundKey)
            plainTextArray2D = self.invMixColumn(plainTextArray2D)
            # print("plain text  :  ", i)
            # print2DArrayHex(plainTextArray2D)

        # last round
        invshiftrows = self.invshiftrows(plainTextArray2D)
        invsubBytes = self.invsubBytes(invshiftrows)
        roundKey = self.arr[0].transpose()
        plainTextArray2D = np.bitwise_xor(invsubBytes, roundKey)
        # print("plain text  round :  ", 0)
        # print2DArrayHex(plainTextArray2D)

        return plainTextArray2D

def initAES(key, keyLen):
    roundLength = 10
    if keyLen == 128:
        roundLength = 10
    elif keyLen == 192:
        roundLength = 12
    elif keyLen == 256:
        roundLength = 14
    aes = AES(roundLength, keyLen)
    allKeysArray = aes.handleKey(key)
    return aes, allKeysArray

def RunAES(aes, plaintext, mode):
    if mode == "encrypt":
        ciphertexts = aes.encryption(plaintext)
        return ciphertexts
    elif mode == "decrypt":
        plaintexts = aes.decryption(plaintext)
        return plaintexts

def getPlainText(plain):
    plaintext = ""
    for pt in plain:
        plaintext += getStringFromHexArray(array2Dto1D(pt))
    return plaintext

def printPlainText(plain):
    #printing decrypted text
    for pt in plain:
        printString(array2Dto1D(pt))
    print()

def RunAES_time_related(plaintext, key):
    #aes initialization
    aes = AES(rounds[0], keys[0])

    #key generation
    start_time_key = time.time()
    allKeysArray = aes.handleKey(key)
    end_time_key = time.time()

    print("Plain Text :\n"+plaintext)
    # print(len(plaintext))
    print(aes.stringtoHex(plaintext))

    print("\nkey :\n"+key)
    print(aes.stringtoHex(key))

    #add padding
    while len(plaintext) % 16 != 0:
        plaintext += ' '

    mainText = plaintext
    #encryption
    ciphertexts = []

    start_time_encrypt = time.time()
    while len(plaintext) != 0:
        pt = plaintext[:16]
        plaintext = plaintext[16:]
        ct = aes.encrypt(pt)
        ciphertexts.append(ct)
    end_time_encrypt = time.time()

    #printing ciphertexts
    print("\nCipher Text :")
    for ct in ciphertexts:
        printArrayHex(array2Dto1D(ct))

    #decryption
    pts = []
    start_time_decrypt = time.time()
    for i in range(len(ciphertexts)):
        ct = ciphertexts[i]
        pt = aes.decrypt(ct)
        pts.append(pt)
    end_time_decrypt = time.time()

    #printing decrypted text
    print("\nDeciphered Text: ")
    decryptedString = ""
    for pt in pts:
        decryptedString += getStringFromHexArray(array2Dto1D(pt))
        printString(array2Dto1D(pt))
    
    print("\n")

    if decryptedString == mainText:
        print("Decryption Successful")
    else:
        print("Decryption Failed")
    
    #priting time details
    print("\nExecution time details: ")
    print("Key Scheduling : ", end_time_key - start_time_key," seconds")
    print("Encryption time : ", end_time_encrypt - start_time_encrypt, " seconds")
    print("Decryption time : ", end_time_decrypt - start_time_decrypt, " seconds")


#round constant
rc = np.array([1, 2, 4, 8, 16, 32, 64, 128, 27, 54, 108, 216, 171, 77, 154])

rounds = np.array([10, 12, 14])
keys = np.array([128, 192, 256])

plaintext = 'CanTheyDoTheirFest'
key = 'BUET CSE17 Batch'

# print("HEre in AES file")

#run AES
# aes, allkeysArray = initAES(key, 128)
# cipher = RunAES(aes, plaintext, "encrypt")
# plain = RunAES(aes, cipher, "decrypt")
# printPlainText(plain)

#print time related data
# RunAES_time_related(plaintext, key)




