#include <stdio.h>
#include <float.h>
int main()
{
	float x=1.0;
	printf("除以0的结果：%f\n", x/0.0);
	printf("除以极小浮点数的结果：%f\n", x/1.4e-45);
	getchar();
	return 0;
}
