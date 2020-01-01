#include <stdio.h>

void cs_itoa(char *s, int x)
{
	int i,j=0;
	char ss[20];
	while(x>0)
	{
		ss[j++] = '0'+x%10;
		x /= 10;
	}
	for(i=j-1; i>=0; i--)
		*s++ = ss[i];
	*s = '\0';
}
int main()
{
	int x;
	char s[20];
	scanf("%d",&x);
	cs_itoa(s, x);
	printf("%s",s);
	return 0;
}
