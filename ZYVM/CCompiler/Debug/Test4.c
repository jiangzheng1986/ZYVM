//�����ṹ���Գ���

unsigned int c=50;

//���Ժ���
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

//������ڵ�
void main(void)
{
    unsigned char a='a';
    unsigned char b='\n';
	char d = 'd';
	int e = 0;
    c=max(a,b);
	e = d;
}