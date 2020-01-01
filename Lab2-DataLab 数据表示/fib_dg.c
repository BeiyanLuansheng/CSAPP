#include <stdio.h>

int fib_int(int n);
long fib_long(int n);
unsigned int fib_uint(int n);
unsigned long fib_ulong(int n);

int main()
{
	int n;
	scanf("%d", &n);
	printf("%d", fib_int(n));
	printf("%ld", fib_long(n));
	printf("%u", fib_uint(n));
	printf("%lu", fib_ulong(n));
	return 0;
}

int fib_int(int n)
{
	if(n==0) return 0;
	else if(n==1) return 1;
	else return fib(n-1)+fib(n-2);
}

long fib_long(int n)
{
	if(n==0) return 0;
	else if(n==1) return 1;
	else return fib(n-1)+fib(n-2);
}

unsigned int fib_uint(int n)
{
	if(n==0) return 0;
	else if(n==1) return 1;
	else return fib(n-1)+fib(n-2);
}

unsigned long fib_ulong(int n)
{
	if(n==0) return 0;
	else if(n==1) return 1;
	else return fib(n-1)+fib(n-2);
}
