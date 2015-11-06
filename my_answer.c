/*************************************************************************
    > File Name: my_answer.c
    > Author: huxingju
    > Mail: 104046058@qq.com 
    > The compiler environment:vim + g++
    > Created Time: 2015年11月05日 星期四 16时37分08秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

Palindrome_num(char *array)			//判断是否为回文数函数
{
	int i=0;
	int j;
	if( (j=strlen(array)-1) < 0 ){
		return -1;						//输入不合法
	}
	if(j==0){
		return 1;						//只有一个数的情况是回文数
	}
	while( (i < j) && array[i] == array[j] ){
		i++; j--;
	}
	if(i<j){
		return 0;						//不是回文数
	}else
		return 1;						//是回文数
}

//十进制转换为其他进制
//ss代表转换者,n代表进制,a存放转化后的字符串
int ten_to_oth(int ss, int n, char a[])
{
	char array[32];				//array中存储未逆置的转换数值
	int i=-1,j=0;
	while(ss != 0)
	{
		i++;
		if( ss%n < 10 ){
			array[i]=ss%n + '0' ;
		}else{
			array[i]='A'+(ss%n-10);
		}
		ss = ss/n;
	}
	array[i+1]='\0';				

	//将转换后的字符串逆置
	while(i>=0){
		a[j++]=array[i--];
	}
	a[j]='\0';
}

//在正整数300范围内查找合适的数并返回十进制数作为密码
int find_number(int n)
{
	int i, sum=0;					//sum用于存放和
	char a[32];						//存放转换后的字符串儿
	int flag=-2;						//标志是否为回文数
	for(i=0; i<= 300 ;i++)
	{
		ten_to_oth(i*i, n, a);			//先进行进制转换
		flag=Palindrome_num(a);			//判断该数是否为回文数，是则相加
		if(flag==1){
			sum += i;
		}
     memset(a, strlen(a)+1, 0);
	}
	return sum;
}

//整理　输入计算并输出
int input_cir_print(void)
{
	int i, n;							//n代表门的扇数
	int m_m, d=1;						//m_m代表所有满足条件的数之和
	char a[32];							//a中暂时存储密码字符串

	printf("输入门的扇数:\n");
	scanf("%d", &n);

	//随机产生1~20的进制数
	srand((unsigned)time(NULL));
	for(i=0; i<n; i++)
	{
		d=rand()%18+2;								//保证1<d<20
		m_m=find_number(d);
		
		ten_to_oth(m_m, d, a);
		printf("%d进制门的密码为：%s\n",d,a);
		memset(a, strlen(a)+1, 0);					//清零函数
	}

	return 0;
}
int main(void)
{
	input_cir_print();
	return 0;
}
