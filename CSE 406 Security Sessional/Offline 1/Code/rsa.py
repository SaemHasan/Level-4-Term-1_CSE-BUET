from utils import *


class RSA:
    def __init__(self):
        self.n = 0
        self.e = 0
        self.d = 0
        return

    def primeNumberGenerator(self, n):
        while True:
            bv = BitVector(intVal=0)
            bv = bv.gen_random_bits(n)
            check = bv.test_for_primality()
            if check>0.99:
                return bv.int_val()

    def gcd(self, a, b):
        a = BitVector(intVal=a)
        b = BitVector(intVal=b)
        return a.gcd(b).int_val()

    def keyGenerator(self, n):
        p = self.primeNumberGenerator(n//2)
        q = self.primeNumberGenerator(n//2)
        # print(p, q)
        n = p*q
        phi = (p - 1) * (q - 1)
        e = 3
        while True:
            if self.gcd(e, phi) == 1:
                break
            else:
                e += 1
        e = BitVector(intVal=e)
        phi = BitVector(intVal=phi)
        d = e.multiplicative_inverse(phi)
        self.n = n
        self.e = e.int_val()
        self.d = d.int_val()
        return [n, e.int_val(), d.int_val()]
    
    def setKeys(self, n, e, d):
        self.n = n
        self.e = e
        self.d = d
        return

    def encrypt(self, plaintext):
        ciphertext = pow(plaintext, self.e, self.n)
        # print(ciphertext)
        return ciphertext

    def decrypt(self, ciphertext):
        plaintext = pow(ciphertext, self.d, self.n)
        # print(plaintext)
        return plaintext

def RunRSA(K, text):
    rsa = RSA()
    
    #key generation
    start_time_key = time.time()
    rsa.keyGenerator(K)
    keyTime = time.time()-start_time_key
    
    #encryption
    start_time_encrypt = time.time()
    a = rsa.encrypt(text)
    encryptTime = time.time()-start_time_encrypt
    
    #decryption
    start_time_decrypt = time.time()
    b = rsa.decrypt(a)
    decryptTime = time.time()-start_time_decrypt
    
    # print(b)
    if b == text:
        print("Decryption Successful. Plaintext: ", text, " Decrypted: ", b)
    else:
        print("Decryption Failed. Plaintext: ", text, " Decrypted: ", b)

    print("K : ", K)
    print("Key Generation Time : ", keyTime, "seconds")
    print("Encryption Time : ", encryptTime, "seconds")
    print("Decryption Time : ", decryptTime, "seconds\n")


Karr = [16,32,64,128]
text = 125

# for i in range(len(Karr)):
#     RunRSA(Karr[i], text)
