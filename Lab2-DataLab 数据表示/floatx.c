#include <stdio.h>
#include<float.h>
void showbytes(unsigned char *pbyte, int len)
{
    int i;
    for(i=0; i<len; i++)
        printf("%.2x ", pbyte[i]);
    printf("\n");
}

int main()
{
	float f0 = +0.0;
	float f1 = -0.0;
	float f2 = 1.4E-45;
	float f3 = FLT_MAX;
	float f4 = FLT_MIN;
	float f5 = 1.0/0.0;
	float f6 = 1.0/0.0*0.0;
	printf("+0：%.50f\n十六进制：",f0);
	showbytes(&f0, sizeof(float));
	
	printf("\n-0：%.50f\n十六进制：",f1);
	showbytes(&f1, sizeof(float));
	
	printf("\n最小浮点正数：%.65f\n十六进制：",f2);
	showbytes(&f2, sizeof(float));
	
	printf("\n最大浮点正数：%.20f\n十六进制：",f3);
	showbytes(&f3, sizeof(float));
	
	printf("\n最小正的规格化浮点数：%.60f\n十六进制：",f4);
	showbytes(&f4, sizeof(float));
	
	printf("\n正无穷大：%.60f\n十六进制：",f5);
	showbytes(&f5, sizeof(float));
	
	printf("\nNnN：%.60f\n十六进制：",f6);
	showbytes(&f6, sizeof(float));

	return 0;
}
