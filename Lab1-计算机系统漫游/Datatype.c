#include <stdio.h>

short height = 173;
int id_i = 1183710113;
long id_l = 1424311999;
char firstname = 'x';
float birth = 19991108;
double id_d = 14243119991108;
int *id_ip = &id_i;
char name[6] = "xujian";
enum {blue, red, yellow}color;

struct
{
    int height;
    double weight;
}info_s;

union
{
    double weight;
    char flag;
}info_u;

void showbytes(unsigned char *pbyte, int len)
{
    int i;
    for(i=0; i<len; i++)
        printf("%.2x ", pbyte[i]);
    printf("\n");
}

int main()
{
    int i=0;
    color = red;
    info_s.height = 173;
    info_s.weight = 56.5;

    info_u.weight = 65.5;
    info_u.flag = 'F';

    printf("类型        变量名        内容                    地址          存储字节\n");
    printf("short       height        %hd                     %p      ", height,&height);
    showbytes(&height, sizeof(height));

    printf("int         id_i          %d              %p      ", id_i, &id_i);
    showbytes(&id_i, sizeof(id_i));

    printf("long        id_l          %ld              %p      ", id_l, &id_l);
    showbytes(&id_l, sizeof(id_l));

    printf("char        firstname     %c                       %p      ", firstname, &firstname);
    showbytes(&firstname, sizeof(firstname));

    printf("float       birth         %f         %p      ", birth, &birth);
    showbytes(&birth, sizeof(birth));

    printf("double      id_d          %f   %p      ", id_d, &id_d);
    showbytes(&id_d, sizeof(id_d));

    printf("int*        id_ip         %d              %p      ", *id_ip, &id_ip);
    showbytes(&id_ip, sizeof(id_ip));

    for(i=0; i<6; i++)
    {
        printf("char[]      name[%d]       %c                       %p      ", i, name[i], &name[i]);
        showbytes(&name[i], sizeof(name[i]));
    }

    printf("union       info_u.weight %f               %p      ", info_u.weight, &info_u.weight);
    showbytes(&info_u.weight, sizeof(info_u.weight));
    printf("union       info_u.flag   %c                       %p      ", info_u.flag, &info_u.flag);
    showbytes(&info_u.flag, sizeof(info_u.flag));

    printf("struct      info_s.height %d                     %p      ", info_s.height, &info_s.height);
    showbytes(&info_s.height, sizeof(info_s.height));
    printf("struct      info_s.height %f               %p      ", info_s.weight, &info_s.weight);
    showbytes(&info_s.weight, sizeof(info_s.weight));

    printf("enum        color         %d                       %p      ", color, &color);
    showbytes(&color, sizeof(color));

    printf("\n函数名      地址\n");
    printf("main        %p\n", &main);
    printf("printf      %p\n", &printf);

    return 0;
}
