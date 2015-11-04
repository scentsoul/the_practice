/*************************************************************************
    > File Name: mm_malloc.c
    > Author: huxingju
    > Mail: 104046058@qq.com 
    > The compiler environment:vim + g++
    > Created Time: 2015年11月03日 星期二 18时58分10秒
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
#include<errno.h>

#define WSIZE 4					//字的大小
#define DSIZE 8					//双字的大小
#define CHUNKSIZE (1<<12)		//扩展堆时的默认大小

#define MAX(x, y) ((x) > (y) ? (x) :(y))

//将大小和已分配位结合起来并返回一个值，可以把它存放在头部或脚部
#define PACK(size, alloc) ((size) | (alloc))

//read and write a word at address p
#define GET(p) (*(unsigned int*)(p))
#define PUT(p,val) (*(unsigned int*)(p)=(val))

//read the size and allocated files from address p
#define GET_SIZE(p) (GET(p) & ~0x7)		//从地址p处的头部或尾部返回大小
#define GET_ALLOC(p) (GET(p) & 0x1)		//从地址p处的头部或尾部返回已分配位

//Give block ptr bp,compute address of its header and footer
#define HDRP(bp) ((char *)(bp)-WSIZE)						//返回指向这个块儿头部指针
#define FTRP(bp) ((char *)(bp)+GET_SIZE(HDRP(bp))-DSIZE)	//尾部

//Give block ptr bp, compute address of next and previous blocks
//
//根据这一块儿的头部确定这块儿的大小从而跳到下一块儿
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE( ((char *)(bp) -WSIZE ) ))		//返回指向后面块的指针

//根据上一块儿的尾部以获得上一块儿的大小从而跳到上一块儿
#define PREV_BLKP(bp) ((char *)(bp) + GET_SIZE( ((char *)(bp)-DSIZE) ))			//返回指向前面块的指针

#define MAX_HEAP (10*(1<<12))		//10MB

/*Private global variables*/
static char *mem_heap;				//Points to first byte of heap
static char *mem_brk;				//Points to last byte of heap plus 1
static char *mem_max_addr;			//Max legal heap addr plus 1
static char *heap_listp;			//指向序言块儿
static void *coalesce(void *bp);	//合并空闲块儿

/*
 *函数声明部分
*/

void mem_init(void);					//申请一大块儿地址空间
void *mem_sbrk(int incr);				//请求额外的堆存储器，拒绝缩堆
int mm_init(void);						//初始化堆Dstatic voia *extend_heap(size_t words);	
static void *extend_heap(size_t words);	//扩展堆
void mm_free(void *bp);					//堆释放
static void *find_fit(size_t asizei);	//首次适配搜索
static void place(void *bp, size_t asize);					//将asize大小的空闲位　置为　已分配位

//mem_init -Initial the memory system model
void mem_init(void)
{
	mem_heap=(char *)malloc(MAX_HEAP);
	mem_brk=(char *)mem_heap;
	mem_max_addr=(char *)(mem_heap + MAX_HEAP);
}

/*mem_sbrk _Simple model of the sbrkfunction.Extends the heap by incr
 * bytes and returns the start address of the new area.In this model,
 * the heap cannot be shrunk
*/
void *mem_sbrk(int incr)
{
	char *old_brk=mem_brk;

	if((incr < 0) || ((mem_brk + incr) > mem_max_addr)){
		errno=ENOMEM;
		fprintf(stderr, "ERROR: mem_sbrk faild. Ran out of memory...\n");
		return (void *)-1;
	}
	mem_brk += incr;
	return (void *)old_brk;
}

//合并空闲块儿
static void *coalesce(void *bp)
{
	size_t prev_alloc=GET_ALLOC(FTRP(PREV_BLKP(bp)));		//获取前面块儿的已分配位
	size_t next_alloc=GET_ALLOC(HDRP(NEXT_BLKP(bp)));		//获取后面块儿的已分配位
	size_t size=GET_SIZE(HDRP(bp));							
	if(prev_alloc && next_alloc){
		return bp;						//前后都被占用   case1 
	}							
	else if(prev_alloc && !next_alloc){
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}									//前一个块儿空闲
	else if(!prev_alloc && next_alloc){
		
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(FTRP(bp), PACK(size, 0));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
	}									//后一个块儿空闲
	else{
		size += GET_SIZE(HDRP(PREV_BLKP(bp)))+GET_SIZE(FTRP(NEXT_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);
	}									//前后块儿都空闲
	return bp;
}

//扩展堆函数
static void *extend_heap(size_t words)
{
	char *bp;
	size_t size;
	
	size = (words % 2) ? (words+1)*WSIZE : words * WSIZE;
	if((long)(bp=mem_sbrk(size)) == -1)
		return NULL;

	//初始化空闲的头尾和结尾块儿
	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0,1) );

	//合并空闲块儿
	return coalesce(bp);
}

// 创建一个带初始空闲块儿的堆
int mm_init(void)
{
	/*create the initial empty heap*/
	if( ( heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1 )
		return -1;

	PUT(heap_listp, 0);										//填充块儿
	PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1));			//序言块儿
	PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1));			//序言块儿
	PUT(heap_listp + (3*WSIZE), PACK(0,1));					//结尾块儿

	/*Extend the empty heap eith a free block of CHUNKSIZE bytes*/
	if(extend_heap(CHUNKSIZE/WSIZE) == NULL)
		return -1;
	return 0;

}

//释放空间
void mm_free(void *bp)
{
	size_t size=GET_SIZE(HDRP(bp));
	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));
	coalesce(bp);
}

/*static void *find_fit(size_t asize)
{
	char *heap_cir=heap_listp;			//指向合适块儿的头
	while(heap_cir < mem_brk){
		if( (GET_ALLOC(heap_cir) ==0) && (GET_SIZE(heap_cir) >= asize) ){
			return heap_cir;
		}
		else 
			heap_cir += GET_SIZE(heap_cir);
	}

	return NULL;
}*/


//对隐式空闲链表进行适配搜索
static void *find_fit(size_t asize)
{
	/*First fit search*/
	void *bp;
	for(bp = heap_listp; GET_SIZE(HDRP(bp)) > 0 ; bp=NEXT_BLKP(bp) ){
		if(!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp))) ){
			return bp;
		}
	}
	return NULL;
}

//将空闲位标志为分配位
static void place(void *bp, size_t asize)
{
	size_t csize=GET_SIZE(HDRP(bp));

	//如果一个块儿不够
	if((csize-asize) >= (2*DSIZE)){

		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		bp=NEXT_BLKP(bp);

		//占用下一个块儿一些空间
		PUT( HDRP(bp), PACK(csize-asize, 0) );	
		PUT( FTRP(bp), PACK(csize-asize, 0) );	
	}else{
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
	}
}

//从空闲链表分配一个块儿
void *mm_malloc(size_t size)
{
	size_t asize;
	size_t extendsize;
	char *bp;

	/*Ignore spurious requests*/
	if(size == 0){
		return NULL;
	}

	/*Adjust block size to include overhead and alignment reqs*/
	if(size <= DSIZE){
		printf("size <= DSIZE\n");
		asize = 2*DSIZE;
	}
	else 
		asize = DSIZE*( (size + (DSIZE) + (DSIZE-1) )/DSIZE );

	//Search the free list for a fit
	if((bp=find_fit(asize)) != NULL){
		place(bp, asize);
		printf("!=NULL  242\n");
		return bp;
	}
	
	extendsize=MAX(asize, CHUNKSIZE);
	if( (bp=extend_heap(extendsize/WSIZE)) == NULL){
		return NULL;
	}
	place(bp, asize);
	return bp;
}


int main(void)
{	printf("哈哈叫我小机智\n");

	size_t a=4;
	int *bp;


	mem_init();
	mm_init();
	bp=(int *)mm_malloc(a);

	if(bp==NULL){
		printf("bp=NULL\n");
		return 0;
	}
	*bp=20;
	printf("%d\n", *bp);
	return 0;
}
