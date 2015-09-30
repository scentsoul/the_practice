/*************************************************************************
    > File Name: palindrome_stack.c
    > Author: huxingju
    > Mail: 104046058@qq.com 
    > The compiler environment:vim + g++
    > Created Time: 2015年09月30日 星期三 09时05分18秒
 ************************************************************************/
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<sys/socket.h>
#include<string.h>

#define FALSE 0
#define TRUE 1
#define stack 40
struct D
{
	char elem[40];				//用于存储回文数字
	int top;					
};

//初始化栈
void init (struct D *s)
{
	s->top = -1;
}

//判栈空
int empty(struct D *s)
{
	if(s->top == -1)
		return TRUE;
	else
		return FALSE;
}

//入栈
int push(struct D *s, char x)
{
	if(s->top == stack-1)
		return FALSE;
	s->top++;
	s->elem[s->top]=x;
	return TRUE;
}

//出栈
int pop(struct D *s, char *p)
{
	if(s->top == -1)
		return		FALSE;

	*p=s->elem[s->top];
	s->top--;

	    return      TRUE;
}

//取栈定元素
int gettop(struct D *s,char *x)
{
	if(s->top == -1)
		return FALSE;
	*x=s->elem[s->top];
	    return TRUE;
}


int input_number(char *array)
{
	int j=0;
	while(1){

		printf("输入数字:\n");
		scanf("%c", &array[j]);
		getchar();

		if(array[j] == '@' || j==38)
			break;
		j++;
	}
	return j-1;

}
//输入并整理数据
void sort_data()
{
	int i=0, j;
	struct D link_a;			//定义结构体变量传参，避免使用二级指针
	struct D link_b;	
	char number_a, number_b;	//暂时保存要比较的数字
	char array[40];				//保存开始输入的数字

	init(&link_a);				//初始化
	init(&link_b);

	j=input_number(array);

	//从两头开始入栈
	while(i < j){

		push(&link_a, array[i]);
		push(&link_b, array[j]);
		i++; j--;
	}

	while(1){
		if( empty(&link_a) || empty(&link_b) )
			break;								//栈空则退出循环
		gettop(&link_a, &number_a);
		gettop(&link_b, &number_b);

		if(number_a != number_b)
			break;
		
		pop(&link_a, &number_a);
		pop(&link_b, &number_b);
	}
	
	if( empty(&link_a) && empty(&link_b) )
		printf("是回文数字\n");
	else
		printf("不是回文数字\n");


}
int main(void)
{
	 sort_data();
	return 0;
}
