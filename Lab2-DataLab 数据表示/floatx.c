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
	printf("+0��%.50f\nʮ�����ƣ�",f0);
	showbytes(&f0, sizeof(float));
	
	printf("\n-0��%.50f\nʮ�����ƣ�",f1);
	showbytes(&f1, sizeof(float));
	
	printf("\n��С����������%.65f\nʮ�����ƣ�",f2);
	showbytes(&f2, sizeof(float));
	
	printf("\n��󸡵�������%.20f\nʮ�����ƣ�",f3);
	showbytes(&f3, sizeof(float));
	
	printf("\n��С���Ĺ�񻯸�������%.60f\nʮ�����ƣ�",f4);
	showbytes(&f4, sizeof(float));
	
	printf("\n�������%.60f\nʮ�����ƣ�",f5);
	showbytes(&f5, sizeof(float));
	
	printf("\nNnN��%.60f\nʮ�����ƣ�",f6);
	showbytes(&f6, sizeof(float));

	return 0;
}
