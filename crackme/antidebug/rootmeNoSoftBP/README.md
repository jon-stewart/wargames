# rootme : No Software Breakpoints

    https://www.root-me.org/en/Challenges/Cracking/ELF-No-software-breakpoints

This is the first good crackme on the rootme site (working through sequentially).  Interesting anti-debugging trick that I will have to remember.

Hand crafted assembly using syscall for IO.  The interesting parts are below:


    [0x08048080]> pdf@entry0
    ...
    |           0x080480ab      b880800408     mov eax, entry0             ; eip
    |           0x080480b0      bb23810408     mov ebx, section_end.LOAD0  ; section_end.LOAD0
    |           0x080480b5      e85b000000     call 0x8048115
    |           0x080480ba      89ca           mov edx, ecx
    |           0x080480bc      b919000000     mov ecx, 0x19               ; 25
    |       .-> 0x080480c1      b855910408     mov eax, 0x8049155
    |       |   0x080480c6      bb88910408     mov ebx, 0x8049188
    |       |   0x080480cb      d1ca           ror edx, 1
    |       |   0x080480cd      8a4408ff       mov al, byte [eax + ecx - 1]
    |       |   0x080480d1      8a5c0bff       mov bl, byte [ebx + ecx - 1]
    |       |   0x080480d5      30d8           xor al, bl
    |       |   0x080480d7      30d0           xor al, dl
    |      ,==< 0x080480d9      751b           jne 0x80480f6
    |      ||   0x080480db      49             dec ecx
    |      |`=< 0x080480dc      75e3           jne 0x80480c1
    ...
    
    [0x08048080]> pd 10 @ 0x8048115
    |              ; CALL XREF from 0x080480b5 (entry0)
    |           0x08048115      29c3           sub ebx, eax
    |           0x08048117      31c9           xor ecx, ecx
    |              ; JMP XREF from 0x08048120 (entry0)
    |       .-> 0x08048119      0208           add cl, byte [eax]
    |       |   0x0804811b      c1c103         rol ecx, 3
    |       |   0x0804811e      40             inc eax
    |       |   0x0804811f      4b             dec ebx
    |       `=< 0x08048120      75f7           jne 0x8048119
    \           0x08048122      c3             ret


Function 0x8048115 generates a key from each byte of the .text segment, program entry point -> end of PT_LOAD0.

    # readelf -hl crackme.bin

    Entry point address:               0x8048080

    Program Headers:
        Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
        LOAD           0x000000 0x08048000 0x08048000 0x00123 0x00123 R E 0x1000
        LOAD           0x000124 0x08049124 0x08049124 0x000a3 0x000a3 RW  0x1000


This is why the challenge is entitled 'no software breakpoints'.  A software breakpoint is achieved by the debugger inserting 0xcc at the start of the instruction - int 3.  The generator function will use 0xcc instead of the correct byte, corrupting the key.


Each byte of the user input/password (0x8049188) is XOR'd against each byte of the intermediate data string (0x8049155) before being XOR'd with the key.

    password ^ intermediate ^ key == 0


To solve this crackme - set a hardware breakpoint.  Except that for me hw breakpoint didn't work in radare2 or gdb.  Instead:

    * Extract .text section from elf
    * Use the algorithm to find the password

    intermediate ^ key == password


Hardware breakpoints indeed rock, except when running x86 on an x64 cpu.


NOTE : This idea could be extended to also prevent the use of hw breakpoints, carry out the computation using each of the DR registers.
