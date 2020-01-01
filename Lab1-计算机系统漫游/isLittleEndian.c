#include <stdio.h>

int isLittleEndian()
{
    int n = 0x12345678;
    char *c = &n;
    if(c[0] == 0x12)
    {
        return 0;
    }
    else return 1;
}

int main()
{
    if(isLittleEndian())
        printf("Little Endian!");
    else printf("Big Endian");
    return 0;
}
