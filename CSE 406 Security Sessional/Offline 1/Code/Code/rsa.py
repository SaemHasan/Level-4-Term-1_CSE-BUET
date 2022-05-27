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
            if check:
                return bv.int_val()

    def gcd(self, a, b):
        a = BitVector(intVal=a)
        b = BitVector(intVal=b)
        return a.gcd(b).int_val()

    def keyGenerator(self, n):
        p = self.primeNumberGenerator(n//2)
        q = self.primeNumberGenerator(n//2)
        print(p, q)
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

    def encrypt(self, plaintext):
        ciphertext = pow(plaintext, self.e, self.n)
        print(ciphertext)
        return ciphertext

    def decrypt(self, ciphertext):
        plaintext = pow(ciphertext, self.d, self.n)
        print(plaintext)
        return plaintext


rsa = RSA()
print(rsa.keyGenerator(128))
a = rsa.encrypt(12345)
b = rsa.decrypt(a)
print(a, b)
