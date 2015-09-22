/*************************************************************************
    > File Name: my_work1.c
    > Author: huxingju
    > Mail: 104046058@qq.com 
    > The compiler environment:vim + g++
    > Created Time: 2015年09月20日 星期日 18时26分39秒
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
	int i;					//代表当前结点表示第几个人
	int number;				//决定下一次要踢人的位置
	struct node *next;
} Nd;

//给每个结点申请空间并赋值
Nd *getNd(int i)
{
	int number;
	Nd *pnew=NULL;

	printf("输入第%d个人手中的编号,0代表停止输入:", i);
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
	pnew->i = i;
	pnew->next=NULL;

	return pnew;
}

//创建循环链表
Nd *createlist()
{
	Nd *pnew=NULL;
	Nd *tail=NULL;
	Nd *head=NULL;
	int i=1;

	pnew=getNd(i);
	head=tail=pnew;

	if(pnew == NULL)	return NULL;	//如果一开始手中的编号就是0就返回
	
	i++;
	pnew=getNd(i);	//申请新结点
	while( pnew != NULL ){
		i++;	
		
		tail->next=pnew;	
		tail=pnew;
		pnew=getNd(i);	//申请新结点
	}

	tail->next=head;
	return head;

}

//对循环链表的输出
void printlist(Nd *head)
{
	Nd *p=head;
	if(head != NULL){
		printf("\n\n自己的编号==手中的数字\n");
		do{
			printf("%3d  %9d\n", p->i, p->number);
			p=p->next;
		}while(p != head);
	}
}
void s_question(Nd *head, int m)
{
	int flag=1, inumber;
	Nd *p=head;			//要删除的结点
	Nd *pre=head;		//要删除的前一个结点
	Nd *del=NULL;		//暂时保存要删除结点的数据

	//控制从第一个人开始删除的情况
	while(pre->next != head)
		pre=pre->next;

	while(flag){
		for(inumber=1; inumber<m; inumber++){
			pre=p;
			p=p->next;
		}

		if(p==pre)			flag=0;
		del=p;
		pre->next=p->next;

		printf("第%d个人出列  ta手中的编号是%d\n", p->i, p->number);
		m=p->number;

		//将p指向将要删除的结点的后一个结点重新开始
		p=p->next;

		free(del);
	}

	head=NULL;
}

int main(void)
{
	Nd *head=NULL;
	int n;

	head=createlist();
	printlist(head);

	printf("输入第一次的编号:\n");
	scanf("%d", &n);
	s_question(head, n);
	return 0;
}
