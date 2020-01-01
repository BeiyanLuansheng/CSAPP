#include <stdio.h>

int utf8len(char *s)
{
    int len=0;
    for(; *s!='\0';)
    {
        if((*s&0x80)==0)
        {
            len++;
            s++;
        }
        else if((*s&0xe0)==0xc0)
        {
            len++;
            s+=2;
        }
        else if((*s&0xf0)==0xe0)
        {
            len++;
            s+=3;
        }
        else if((*s&0xf8)==0xf0)
        {
            len++;
            s+=4;
        }
    }
    return len;
}

int main()
{
    char s[100];
    scanf("%s",&s);
    int len=utf8len(&s);
    printf("%d",len);
    return 0;
}
