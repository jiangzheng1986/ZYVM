//基础结构测试程序

//测试函数
int max(int a,int b)
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

int c=50;

//程序入口点
void main(void)
{
    int a=10;
    int b=20;
    c=max(a,b);
}