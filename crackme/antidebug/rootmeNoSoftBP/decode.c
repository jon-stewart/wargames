#include <stdio.h>
#include <stdlib.h>

static unsigned char data[] = {0x1e, 0xcd, 0x2a, 0xd5 , 0x34, 0x87, 0xfc, 0x78, 0x64, 0x35, 0x9d, 0xec, 0xde, 0x15, 0xac, 0x97, 0x99, 0xaf , 0x96, 0xda, 0x79, 0x26, 0x4f, 0x32, 0xe0};

static unsigned int key;

static int text_len = 0xa3;

static inline int
rol(unsigned int val, unsigned int shift) {
    if ((shift &= sizeof(val) * 8 - 1) == 0)
        return val;

    return (val << shift) | (val >> (sizeof(val) * 8 - shift));
}

void
generate_cypher(void)
{
    FILE           *fp;
    unsigned char   buf[text_len];
    int             i;

    fp = fopen("text_section", "rb");

    fread(buf, sizeof (char), text_len, fp);

    fclose(fp);

    for (i = 0; i < text_len; ++i) {
        key = (0xFFFFFF00 & key) + (0xFF & (key + buf[i]));

        key = rol(key, 3);
    }
}

void
decode(void)
{
    unsigned char   password[25];
    int             i, j = 0;

    for (i = 24; i >= 0; --i) {
        cypher >>= 1;

        password[i] = data[i] ^ ((unsigned char) key);
    }

    printf(": %s\n", password);
}

int
main(void)
{
    generate_cypher();

    decode();

    return (0);
}
