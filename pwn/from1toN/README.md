## Re: 从一开始的 orw 生活 (3 solves)

### Description

对 master 来说，一个 syscall 就能 orw 了 🤔

### 解题思路

本题是 `io_uring` 编程题, 高版本 (`Linux Kernel Version >= 6.5`) 的 `io_uring` 中引入了 `IORING_SETUP_NO_MMAP` 标志，配合 `IORING_SETUP_SQPOLL` 使得一次 syscall 完成 orw 操作成为现实。

由于出题人一时脑子短路 (😭) 把 `close({0, 1, 2})`  放在了执行 shellcode 之后，使得选手做题时直接省去了 `io_socket` 的步骤 ，不过好在区别不大。（多加几个 sqe 的事情 orz）


