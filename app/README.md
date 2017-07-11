# app

GDB:

gdb will add additional items to the environmental variables, changing stack addressing.  Remove with:

    (gdb) unset env LINES
    (gdb) unset env COLUMNS


To enter binary while in gdb:

    python2.7 -c 'print ..') > /tmp/input

    gdb> r < /tmp/input


C++:

GNU g++ places vtable at end of object.

Windows visual C++ places vtable at start of object.


Piped shell:

    (python -c 'print ..' ; /bin/echo "<command>") | ./vulnerable
