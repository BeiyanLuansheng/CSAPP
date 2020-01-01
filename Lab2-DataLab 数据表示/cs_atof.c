#include <stdio.h>

double cs_atof(char s[])
{
	int x, i;
	double f;
	x = s[0]-48;
	for(i=1;s[i]!='.';i++)
		x = x*10+(s[i]-48);
	f = (double)x;
	double tp=0.1;
	for(i++; s[i]!='\0'; i++)
	{
		f = f+tp*(double)(s[i]-48);
		tp /= 10;
	}
	return f;
}

int main()
{
	char s[50];
	scanf("%s",&s);
	double x;
	x = cs_atof(s);
	printf("%f",x);
	return 0;
}
