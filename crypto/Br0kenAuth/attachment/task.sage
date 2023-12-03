from hashlib import sha256
from base64 import b64encode as b64e, b64decode as b64d
from secret import n
import os
FLAG = os.environ.get('FLAG', 'flag{XXXXFAKE_FLAGXXXX}')

class HECDSA:
    def __init__(self, q):
        self.q = q
        self.sk = randint(0, self.q)
        self.G, self.J = self.curve_init()
        assert self.q*self.G == self.J.an_element()
        self.P = self.sk*self.G
        self.fault_mod = self.q^^1

    def curve_init(self):
        f = x^7+x^5+x+1
        sy = f(F.fetch_int(31337)).nth_root(2)
        HC = HyperellipticCurve(f, 0)
        J = HC.jacobian()(F)
        return J(HC(F.fetch_int(31337), sy)), J
    
    def encode(self, c):
        x, y = c
        px = PR(list(map(F.from_integer, x)))
        py = PR(list(map(F.from_integer, y)))
        return self.J((px, py))
    
    def decode(self, c):
        x, y = c
        x = [i.to_integer() for i in x]
        y = [i.to_integer() for i in y]
        return x, y

    def fault_sign(self, msg):
        e = int(sha256(msg.encode()).hexdigest(), 16)
        k = randint(0, self.q)
        r = k*self.G
        r_ = int(sha256(str(r).encode()).hexdigest(), 16)%self.fault_mod
        s = inverse_mod(k, self.fault_mod)*(e+r_*self.sk)%self.fault_mod
        return (*self.decode(r), s)

    def verify(self, msg, sig):
        rx, ry, s = sig
        s = int(s)
        r = self.encode((rx, ry))
        r_ = int(sha256(str(r).encode()).hexdigest(), 16)%self.q
        e = int(sha256(msg.encode()).hexdigest(), 16)
        if s*r == e*self.G+r_*self.P:
            return True
        return False
    
def menu():
    MENU = """
            /----------------------------\\
            |          options           |
            | [G]enrate Token            |
            | [L]ogin                    |
            \\----------------------------/
            """
    print(MENU)
    
def print_token(sig):
    token = "Your Token: "
    for i in sig:
        token += '#'
        token += b64e(str(i).encode()).decode()
    print(token)
    
def input_token(token):
    token = token.split('#')[1:]
    sig = []
    for obj in token[:-1]:
        sig.append(b64d(obj).decode().strip('[]').split(','))
    sig.append(b64d(token[-1]).decode())
    return sig
        
def Auth():
    menu()
    while True:
        op = input("choice > ").strip()
        if op == 'G':
            username = input("Name: ").strip('')
            if username == "admin":
                print("No can do...")
            else:
                while True:
                    try:
                        signature = signer.fault_sign(username)
                        print_token(signature)
                        break
                    except:
                        continue
        elif op == 'L':
            username = input("Name: ").strip()
            token = input("Token: ").strip()
            if signer.verify(username, input_token(token)):
                if username == "admin":
                    print(f"Hi, admin. Your flag here {FLAG}!")
                else:
                    print(f"Hi, {username}")
            else:
                print("Wrong Token!")


if __name__ == "__main__":
    try:
        F = GF(3^71)
        PR.<x> = PolynomialRing(F)
        signer = HECDSA(n)
        Auth()
    except:
        print("error!")