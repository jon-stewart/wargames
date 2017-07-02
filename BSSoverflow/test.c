#include <stdio.h>
#include <stdlib.h>

char *code = "";

int
main(void)
{
    int *ret;

    ret = (int *) &ret + 2;

    (*ret) = (int) code;

    return (0);
}
