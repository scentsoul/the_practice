/*************************************************************************
    > File Name: test_stack.c
    > Author: huxingju
    > Mail: 104046058@qq.com 
    > The compiler environment:vim + g++
    > Created Time: 2015年11月17日 星期二 16时21分01秒
 ************************************************************************/

/*程序内容: 算数表达式求值*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FALSE 0
#define TRUE 1

/*宏定义加减乘除*/
#define ADDI 43
#define SUBT 45
#define MULT 42
#define DIVI 47

/*宏定义大于小于等于*/
#define LARG 62
#define LESS 60
#define EQUA 61

/*宏定义() 和 #*/
#define LEFT 40
#define RIGHT 41
#define KEY 35

//数据结构:链栈
typedef struct my_Istack
{
	int data;
	struct my_Istack *next;
}my_Istack;

//链栈头
my_Istack *Init_top()
{
	my_Istack *s = NULL;
	if( (s=(my_Istack *)malloc(sizeof(my_Istack)) ) == NULL){
		printf("申请内存空间错误\n");
		exit(-1);
	}
	s->next = NULL;
	s->data=0;

	return s;
}

//取栈顶元素
int my_get_top(my_Istack *head, int *x)
{
	if(head->next == NULL)
		return FALSE;					//空栈直接返回

	*x = (head->next) -> data;
	return TRUE;
}

//入栈
int my_push_stack(my_Istack *head, int x)
{
	my_Istack *s=NULL;
	if( (s=(my_Istack *)malloc(sizeof(my_Istack) ) ) == NULL )
		return FALSE;
	s->data=x;
	s->next=head->next;
	head->next=s;
	return TRUE;
}

//出栈
int my_pop_stack(my_Istack *head, int *x)
{
	my_Istack *p=NULL;
	if(head->next == NULL)
		return FALSE;					//空栈直接返回
	
	p=head->next;
	head->next=p->next;
	*x = p->data;
	free(p);
	return TRUE;
}

//判断是操作数还是运算符
int my_check(char ch)
{
	if(ch >= '0' && ch <= '9')
		return FALSE;
	switch(ch)
	{
		case ADDI:
		case SUBT:
		case MULT:
		case DIVI:
		case LEFT:
		case KEY:
		case RIGHT:
			return TRUE;
		default :
			return -1;
	}
}

//优先级比较
//注意:为了简化程序，这里的优先级与实际数学里面的优先级有差异
int  my_compare(char ch, char ch1)
{
	if(ch == ADDI || ch == SUBT )
	{
		if( (ch1 == ADDI) || (ch1 == SUBT) || (ch1 == RIGHT )  || (ch1 == KEY) )
			return LARG;
		else if(ch1 == MULT || ch1 == DIVI || ch1 == LEFT )
			return LESS;
	}
	else if(ch == MULT || ch == DIVI)
	{
		if(ch1 == LEFT)
			return LESS;
		return LARG;
	}
	else if(ch == LEFT)
	{
		if(ch1 == RIGHT )
			return EQUA;
		else if(ch1 != KEY)
			return LESS;
	}
	else if(ch == RIGHT )
	{
		if(ch1 != LEFT )
			return LARG;
	}
	else if(ch == KEY)
	{
		if(ch1 == KEY)
			return EQUA;
		else if(ch1 != RIGHT)
			return LESS;
	}
	else 
		return -18;			//特殊值

}

//值进行算数运算
int my_test(int a, int b, char op)
{
	if(op == ADDI)	return (a+b);
	if(op == SUBT)	return (a-b);
	if(op == MULT)	return (a*b);

	if(op == DIVI && b != 0)	
		return (a/b);
}

//计算功能实现函数
int my_calculate()
{
	my_Istack *OPRD=NULL;		//操作数栈
	my_Istack *OPTR=NULL;		//运算符栈

	char ch;					//用于暂时保存输入的值
	int data=0;					//暂存操作数
	int op=0;					//获顶取栈运算符
	int a, b, val=1;			//暂存运算的两个数据和运算结果

	OPRD = Init_top();
	OPTR = Init_top();				//栈初始化

	my_push_stack(OPTR, KEY);
	ch=getchar();

	my_get_top(OPTR, &op);
	while( (ch != KEY) || (op != KEY)  )
	{
		//如果是操作数
		if(my_check(ch) == FALSE){
			data=ch-'0';
			
			ch=getchar();
			while(my_check(ch) == FALSE){
				data = data*10 + (ch-'0');
				ch = getchar();

			}
			my_push_stack(OPRD, data);
		}
		
		//如果是运算符
		else if(my_check(ch) == TRUE){
			my_get_top(OPTR, &op);

			switch( my_compare(op, ch) )
			{
				//新输入运算符优先级高于栈顶运算符 ，压入栈底
				case LESS:
					my_push_stack(OPTR, ch);
					ch=getchar();
					break;

				//新输入运算符优先级等于栈顶运算符,说明左右括号相遇，做＝左括号出栈
				case EQUA:
					my_pop_stack(OPTR, &op);
					ch=getchar();
					break;

				//新输入运算符优先级低于栈顶运算符,栈顶运算符出栈,计算
				case LARG:
					my_pop_stack(OPTR, &op);
					my_pop_stack(OPRD, &a);
					my_pop_stack(OPRD, &b);

					val = my_test(b, a, op);
					my_push_stack(OPRD, val);
					break;
			}

		}
		 my_get_top(OPTR, &op);
	}

	my_get_top(OPRD, &val);
	return val;

}

int main(void)
{
	int val=0;
/*	char ch=')';
	printf("%d\n%d\n%d\n%d\n%d\n%d", '+','-','*','/','(',')' );
	printf("\n\n%d\n%d\n%d\n%d\n", '>','<','=','\n');

	if(RIGHT == ch)
		printf("yes\n");*/
	
	printf("输入表达式, 并以#键结尾\n");
	printf("例如:\ninput:1+2*(3+4)#\ninput:");
	val=my_calculate();
	printf("结果为:%d\n", val);
	return 0;
}
