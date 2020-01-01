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
	int i, x=1, y, fib=0;
	for(i=0; i<n; i++)
	{
		y=fib;
		fib += x;
		x=y;
	}
	return fib;
}

long fib_long(int n) 
{
	int i; 
	long fib=0, x=1, y;
	for(i=0; i<n; i++)
	{
		y=fib;
		fib += x;
		x=y;
	}
	return fib;
}

unsigned int fib_uint(int n)
{
	int i;
	unsigned int fib=0, x=1, y;
	for(i=0; i<n; i++)
	{
		y=fib;
		fib += x;
		x=y;
	}
	return fib;
}

unsigned long fib_ulong(int n)
{
	int i;
	unsigned long fib=0, x=1, y;
	for(i=0; i<n; i++)
	{
		y=fib;
		fib += x;
		x=y;
	}
	return fib;
}
