from pwn import *
from base64 import b64encode as b64e, b64decode as b64d
from hashlib import sha256
from tqdm import tqdm
context(log_level="debug")

F = GF(3^71)
PR.<x> = PolynomialRing(F)
f = x^7+x^5+x+1
HC = HyperellipticCurve(f, 0)
J = HC.jacobian()(F)
C = HC.change_ring(GF(3))
p = C.frobenius_polynomial()
alpha = p.roots(ring=QQbar)
N = ZZ(prod(1-a[0]**71 for a in alpha))
sy = f(F.fetch_int(31337)).nth_root(2)
G = J(HC(F.fetch_int(31337), sy))

def bsgs(Q, P, bound):
    dic = {}
    step = isqrt(bound)+1
    start = J.an_element()
    for i in tqdm(range(step)):
        dic[str(Q-start)] = i
        start += P
    start = J.an_element()
    sP = step*P
    for i in tqdm(range(step)):
        if str(start) in dic.keys():
            return i*step+dic[str(start)]
        start += sP
    
def dlog(Q, P):
    d = []
    n = [83, 3739429]
    for _ in range(2):
        Q0 = N//n[_]*Q
        P0 = N//n[_]*P
        d.append(bsgs(Q0, P0, n[_]))
    return crt(d, n)

def encode(c):
    x, y = c
    px = PR(list(map(F.from_integer, x)))
    py = PR(list(map(F.from_integer, y)))
    return J((px, py))

def decode(c):
    x, y = c
    x = [i.to_integer() for i in x]
    y = [i.to_integer() for i in y]
    return x, y
    
def get_data(token):
    token = token.split('#')[1:]
    sig = []
    for obj in token[:-1]:
        sig.append(b64d(obj).decode().strip('[]').split(','))
    sig.append(b64d(token[-1]).decode())
    rx, ry, s = sig
    s = int(s)
    r = encode((rx, ry))
    t = dlog(r, G)
    r_ = int(sha256(str(r).encode()).hexdigest(), 16)%(N-1)
    print(s, N-1, GCD(s, N-1))
    if GCD(s, N-1) == 1:
        rl.append(r_); sl.append(s); tl.append(t)
        
def sign(msg, sk):
    e = int(sha256(msg.encode()).hexdigest(), 16)
    k = randint(0, N)
    r = k*G
    r_ = int(sha256(str(r).encode()).hexdigest(), 16)%N
    s = inverse_mod(k, N)*(e+r_*sk)%N
    return (*decode(r), s)


def gen_token(sig):
    token = ""
    for i in sig:
        token += '#'
        token += b64e(str(i).encode()).decode()
    return token

rl, sl, tl = [[] for _ in range(3)]
e = int(sha256(b'hash_hash').hexdigest(), 16)
dim = 14
io = remote("30.222.0.103", '1337')
token = []
for _ in range(dim+20):
    io.sendlineafter("choice > ", 'G')
    io.sendlineafter("Name: ", 'hash_hash')
    io.recvuntil("Your Token: ")
    token.append(io.recvline().decode()[:-1])

print("BSGS")
idx = 0
while len(rl) < dim:
    get_data(token[idx])
    idx += 1

n = 3739429*83
A, B = [], []
ni = inverse_mod(n, (N-1))
for _ in range(dim):
    A.append(ni*inverse_mod(sl[_], (N-1))*rl[_]%(N-1))
    B.append((ni*inverse_mod(sl[_], (N-1))*e-ni*tl[_])%(N-1))

L = matrix(QQ, dim+2, dim+2)
for _ in range(dim):
    L[_, _] = N-1
    L[dim, _] = A[_]
    L[dim+1, _] = B[_]
    
L[dim, dim] = 1/2^29
L[dim+1, dim+1] = 2^(338-29)
basis = L.LLL()

def forge(sk):
    tk = gen_token(sign('admin', sk))
    io.sendlineafter("choice > ", 'L')
    io.sendlineafter("Name: ", 'admin')
    io.sendlineafter("Token: ", tk)

for item in basis:
    if abs(item[-1]) == 2^(338-29):
        sk1 = (N-1)-abs(item[-2])*2^29
        sk2 = abs(item[-2])*2^29
        forge(sk1)
        forge(sk2)
        
io.interactive()