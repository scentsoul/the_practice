/*************************************************************************
	irintlist(head);
    > File Name: work6.c
    > Author: huxingju
    > Mail: 104046058@qq.com 
    > The compiler environment:vim + g++
    > Created Time: 2015年09月21日 星期一 07时30分19秒
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
typedef struct node
{
	int number;			//链表数据域的数据
	struct node *next;
}Nd;

Nd *getNd()
{
	int number;
	Nd *pnew=NULL;

	printf("输入数据输入0时退出:\n");
	scanf("%d",&number);

	//避免多申请一个内存空间
	if(number == 0){
		pnew=NULL;
		return pnew;
	}

	//申请内存空间
	if( (pnew=(Nd *)malloc( sizeof(Nd) ) ) == NULL){
		printf("申请内存错误\n");
		exit(-1);
	}
	pnew->number=number;
	pnew->next=NULL;

	return pnew;
}

//循环链表的释放
void my_free(Nd *head)
{
	Nd *p=head;
	Nd *pre=NULL;
	head=head->next;
	while(head != p)
	{
		pre=head;
		head=head->next;
		free(pre);
	}

}
//创建带头结点的循环链表
Nd *createlist()
{
	Nd *pnew=NULL;
	Nd *tail=NULL;
	Nd *head=NULL;

	pnew=getNd();

	head=(Nd *)malloc(sizeof(Nd));
	head->number=0;

	tail=pnew;
	head->next=tail;
	
	if(pnew == NULL)	return NULL;	//如果一开始手中的编号就是0就返回
	
	pnew=getNd();	//申请新结点
	while( pnew != NULL ){
		
		tail->next=pnew;	
		tail=pnew;
		pnew=getNd();	//申请新结点
	}
	tail->next=head;

	return head;
}

Nd *sort_AB(Nd **A, Nd **B)
{
	Nd *C=NULL, *tail=NULL;
	Nd *headC=NULL;

	*A=(*A)->next;
	*B=(*B)->next;
	
	//给c链表申请头结点
	if(  (headC= (Nd* ) malloc ( sizeof(Nd) ) ) == NULL){
		printf("申请内存空间出错\n");
		exit(-1);
	}

	headC->number=0;
	tail=headC;

	//将A和B链的数据链给C链，并将已处理的结点释放
	C=(Nd *)malloc( sizeof(Nd) );

	//两条链表头结点所指向的number值都是0用来标志是否为头结点
	while( (*A)->number  &&  (*B)->number ){

		if((*A)->number <= (*B)->number){

			C->number=(*A)->number;
			*A=(*A)->next;
		}else{
			
			C->number=(*B)->number;
			*B=(*B)->next;
		}
		
		tail->next=C;
		tail=C;
		C=(Nd *)malloc( sizeof(Nd) );
	}

	//A或者B有剩余的直接连在C链后面
	while( (*A)->number){
		
		C->number=(*A)->number;
		*A=(*A)->next;
		tail->next=C;
		tail=C;
		C=(Nd *)malloc( sizeof(Nd) );
	}
	while( (*B)->number ){

		C->number=(*B)->number;
		*B=(*B)->next;
		tail->next=C;
		tail=C;
		C=(Nd *)malloc( sizeof(Nd) );
	}


	tail->next=headC;

	return headC;
}
//对循环链表的输出
void printlist(Nd *head)
{
	Nd *p=head->next;
	if(head != NULL){
		printf("输出\n");
		do{
			printf("%3d\n", p->number);
			p=p->next;
		}while(p != head);
	}
}


int main(void)
{
	Nd *headA=NULL, *headB=NULL;
	Nd *headC=NULL;

	int n;

	headA=createlist();
	printlist(headA);

	headB=createlist();
	printlist(headB);
	
	headC=sort_AB(&headA, &headB);
	printlist(headC);


	return 0;
}
