#include <stdio.h>

unsigned unsigned_high_prod(unsigned x, unsigned y)
{
    int w = sizeof(int) << 3;
    unsigned high = 0;
    unsigned sum = (y&0x1) ? x : 0;
    int i;
    for(i=1; i<w; i++){
        if((y >> i) & 0x1){
            high += x >> (w-i);
            if(!(sum + (x << i) >= sum)) high++;
            sum += (x << i);
        }
    }
    return high;
}

int main(){
	unsigned x,y;
	scanf("%u,%u",&x,&y);
	unsigned high = unsigned_high_prod(x,y);
	printf("高w位（10进制）：%u", high);
}

