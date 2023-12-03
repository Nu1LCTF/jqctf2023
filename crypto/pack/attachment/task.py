from Crypto.Util.number import *
import random
import os
FLAG = os.environ.get('FLAG', 'flag{XXXXFAKE_FLAGXXXX}')
assert FLAG[:5] == 'flag{' and FLAG[-1] == '}'
FLAG = FLAG[5:-1]

def pack(dim):
    sc = [-1, 1]
    a = [getPrime(256) for _ in range(dim-1)]
    s = [sc[random.randint(0, 1)] for _ in range(dim)]
    a.append(-s[-1]*sum(a[i]*s[i] for i in range(dim-1)))
    return a, s

def epack(error, dim):
    a, s_ = pack(dim)
    s = [i if i>0 else i+1 for i in s_]
    for _ in range(dim):
        s[_] ^= int(error[_])
    return a, sum(a[_]*s[_] for _ in range(dim))

if __name__ == "__main__":
    bits = bin(bytes_to_long(FLAG.encode()))[2:].zfill(len(FLAG)*8)
    for _ in range(2):
        print(epack(list(bits), len(bits)))