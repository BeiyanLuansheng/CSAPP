#include <stdio.h>

typedef unsigned float_bits;
float_bits float_half(float_bits f){
	unsigned flag = f >> 31;
	unsigned x = (f >> 23) & 0xff;
	unsigned y = f & 0x7fffff;
	if(x==0xff) return f;
	else if(x==0) return (flag << 31) | ((y>>1) + (1 & (y&3==3)));
	else if(x==1) return (flag << 31) | ((x-1) << 23) | ((y>>1) & 0x400000);
	else return (flag << 31) | ((x-1) << 23) | y;
}

int main(){
	float_bits f = 0;
	float_bits result;
	int i=0;
	for(; f<4294967296; f++){
		result = float_half(f);
		printf("%u\n",(unsigned)result);
	}
	return 0;
}
