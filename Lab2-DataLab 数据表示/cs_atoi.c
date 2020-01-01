#include <stdio.h>

int cs_atoi(char s[])
{
	int x,i;
	x = s[0]-48;
	for(i=1;s[i]!='\0';i++)
		x = x*10+(s[i]-48);
	return x;
}

int main()
{
	char s[50];
	scanf("%s",&s);
	int x;
	x=cs_atoi(s);
	printf("%d",x);
	return 0;
}
