#include <stdio.h>

void cs_ftoa(char *s, double x)
{
	int integ, i=0, j=0;
	double frac;
	char *sstr = s;
	char ss[20];
	integ = (int)x;
	frac = x-integ;
	x = frac;
	while(integ>0)
	{
		ss[j++] = '0'+integ%10;
		integ /= 10;
	}
	for(i=j-1; i>=0; i--)
		*s++ = ss[i];
	double tp=0.1;
	*s++ = '.';
	while(x > 1e-6)
	{
		x -= tp * (int)(frac*10);
		tp /= 10;
		*s++ = '0'+(int)(frac*10);
		frac = frac*10 - (int)(frac*10);
	}
	*s='\0';
}

int main()
{
	double x;
	char s[20];
	scanf("%lf", &x);
	cs_ftoa(s, x);
	printf("%s",s);
	return 0;
}
