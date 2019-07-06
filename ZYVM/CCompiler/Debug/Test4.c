//基础结构测试程序

unsigned int c=50;

//测试函数
unsigned char max(unsigned char a,unsigned char b)
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

//程序入口点
void main(void)
{
    unsigned char a='a';
    unsigned char b='\n';
	char d = 'd';
	int e = 0;
    c=max(a,b);
	e = d;
}