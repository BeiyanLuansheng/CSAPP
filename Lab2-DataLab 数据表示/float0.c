#include <stdio.h>
#include <float.h>
int main()
{
	float x=1.0;
	printf("����0�Ľ����%f\n", x/0.0);
	printf("���Լ�С�������Ľ����%f\n", x/1.4e-45);
	getchar();
	return 0;
}
