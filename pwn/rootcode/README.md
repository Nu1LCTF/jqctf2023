## rootcode (13 solves)

### Description

Since you have experienced many shellcode master questions, now try my rootcode chal!

### 解题思路

1. 利用栈上残留地址，计算出内核机制，执行 `commit_creds(&init_cred)` 提权
2. 利用 gs 相对偏移，取出 `current_task` 的 `current_cred` ，修改掉 `cred` 进行提权

由于是 call 调用 shellcode，无需专门自行处理返回用户态，执行完毕 shellcode 后直接 ret 返回到 ioctl 流程即可返回。


