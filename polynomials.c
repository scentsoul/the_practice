/*************************************************************************
    > File Name: my_work2.c
    > Author: huxingju
    > Mail: 104046058@qq.com 
    > The compiler environment:vim + g++
    > Created Time: 2015年09月20日 星期日 21时46分59秒
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
	double xcount;		//系数
	int	   zcount;		//指数
	struct node *next;	
}Nd;

//给每个结点申请空间并赋值
Nd *getNd()
{
	double xcount;		//系数
	int	   zcount;		//指数
	Nd *pnew=NULL;

	printf("\n输入系数:");
	scanf("%lf",&xcount);

	//避免多申请一个内存空间
	if(xcount == 0){
		pnew=NULL;
		return pnew;
	}

	printf("输入指数:");
	scanf("%d",&zcount);

	if( (pnew=(Nd *)malloc( sizeof(Nd) ) ) == NULL){
		printf("申请内存错误\n");
		exit(-1);
	}

	pnew->xcount=xcount;
	pnew->zcount=zcount;
	pnew->next=NULL;

	return pnew;
}
void insert_list(Nd ** head, Nd **pnew)
{
	Nd *pre=NULL;
	pre=*head;
	while(pre->next != NULL && (pre->next)->zcount < (*pnew)->zcount ){
		pre = pre->next;
	}
	
	if(pre->next == NULL){						//直接在尾巴插入

		pre->next=*pnew;
		pre=*pnew;
		pre->next=NULL;
	}else if((pre->next)->zcount == (*pnew)->zcount ){
		
		(pre->next)->xcount +=(*pnew)->xcount;
	}else{
		
		(*pnew)->next=pre->next;
		pre->next=(*pnew);
	}
		
}
Nd *createlist()
{
	Nd *head=NULL;								//头结点
	Nd *pnew=NULL;								//新申请的和尾结点
	
	head=(Nd *)malloc(sizeof(Nd));				//建立头结点
	head->xcount=0;
	head->next=NULL;

	pnew=getNd();		pnew->next=NULL;
	if(pnew != NULL)	head->next=pnew;		//将头结点和后面的连接起来
	
	pnew=getNd();								//重新申请
	while(pnew != NULL)
	{	
		insert_list(&head, &pnew);				//插入新数据到已有链表中
		pnew=getNd();	
	}

	free(pnew);
	return head;
}


void printlist(Nd *head)						//输出数据函数
{
	Nd *p=head->next;
	while( p != NULL){
		printf("xcount=%lf      zcount=%d\n", p->xcount, p->zcount);
		p=p->next;
	}
}


Nd * add_poly(Nd *headA, Nd *headB)				//多项式相加函数
{
	Nd *headC=NULL, *tail=NULL, *pnew=NULL;
	headA=headA->next;
	headB=headB->next;

	headC=(Nd *)malloc(sizeof(Nd));
	headC->next=NULL;
	headC->xcount=0;

	tail=headC;
	while(headA != NULL && headB != NULL)
	{
		pnew=(Nd *)malloc(sizeof(Nd));
		pnew->next=NULL;

		//如果链A的指数小于等于链B的指数
		if(headA->zcount < headB->zcount){
			pnew->xcount=headA->xcount;
			pnew->zcount=headA->zcount;
			headA=headA->next;
		}
	
		//如果链A和链B的指数相等则加上B的系数再把B链的指向向后移一个
		else if(headA->zcount == headB->zcount){

			pnew->xcount = headB->xcount + headA->xcount;
			pnew->zcount=headA->zcount;

			headA=headA->next;
			headB=headB->next;
		}

		//如果链A的指数大于链B的指数
		else if(headA->zcount > headB->zcount){
			pnew->xcount=headB->xcount;
			pnew->zcount=headB->zcount;
			headB=headB->next;
		}

		tail->next=pnew;
		tail=pnew;
	}

	//如果A链有剩余的
	while(headA != NULL){
		pnew=(Nd *)malloc(sizeof(Nd));
		pnew->next=NULL;

		pnew->xcount=headA->xcount;
		pnew->zcount=headA->zcount;
		headA=headA->next;

		tail->next=pnew;
		tail=pnew;

	}
	
	//如果B链有剩余的
	while(headB != NULL){
		pnew=(Nd *)malloc(sizeof(Nd));
		pnew->next=NULL;

		pnew->xcount=headB->xcount;
		pnew->zcount=headB->zcount;
		headB=headB->next;

		tail->next=pnew;
		tail=pnew;
	}

	return headC;

}

Nd *multi(Nd *headA,Nd *headB)									//多项式相乘函数
{
	Nd *trav1=headA->next, *trav2=NULL;							//两条链表循环
	Nd *headC=NULL;												//c链表的头结点
	Nd *pnew=NULL;												//用于创建一条相乘结果的链表C

	if( (headC=(Nd *)malloc(sizeof(Nd))) == NULL){
		printf("内存申请错误\n");	exit(-1);
	}
	headC->zcount=0;
	headC->next=NULL;

	while(trav1 != NULL){
		
		trav2=headB->next;
		while(trav2 != NULL){	
			
			pnew=(Nd *)malloc(sizeof(Nd));
			pnew->next=NULL;

			pnew->xcount = trav1->xcount * trav2->xcount;		//给要插入的结点赋值
			pnew->zcount = trav1->zcount + trav2->zcount;

			insert_list(&headC, &pnew);							//插入新数据到C链表中
			trav2=trav2->next;
		}
		trav1=trav1->next;
	}

	return headC;
}

int main(void)
{
	Nd *headA=NULL;
	Nd *headB=NULL;
	Nd *headC=NULL;

	headA=createlist();
	printf("原链表:\n");
	printlist(headA);
	
	headB=createlist();
	printf("原链表:\n");
	printlist(headB);

	printf("===============\n\n");

	printf("多项式相加的结果\n\n");
	headC=add_poly(headA, headB);
	printlist(headC);


	printf("\n\n多项式相乘的结果\n");
	headC=multi(headA,headB);
	printlist(headC);



}
