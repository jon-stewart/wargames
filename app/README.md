# app

GDB:

gdb will add additional items to the environmental variables, changing stack addressing.  Remove with:

    (gdb) unset env LINES
    (gdb) unset env COLUMNS
