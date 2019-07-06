//基础结构测试程序

//测试函数
char max(char a,char b)
{
    if(a>b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

char c=50;

//程序入口点
void main(void)
{
    char a='a';
    char b='\n';
    c=max(a,b);
}