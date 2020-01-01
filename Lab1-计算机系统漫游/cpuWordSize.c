#include <stdio.h>

int cpuWordSize()
{
    void *p = 0;
    return sizeof(&p);
}
int main()
{
    printf("CPU word size: %d\n",cpuWordSize());
    return 0;
}
