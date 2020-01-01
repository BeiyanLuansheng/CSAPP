#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "1837101",
    /* First member's full name */
    "xujian",
    /* First member's email address */
    "1183710113@stu.hit.edu.cn",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

/* $begin mallocmacros */
/* Basic constants and macros */
#define WSIZE       4       /* word size (bytes) */
#define DSIZE       8       /* doubleword size (bytes) */
#define CHUNKSIZE  (1<<11)  /* initial heap size (bytes) */
#define OVERHEAD    8       /* overhead of header and footer (bytes) */
#define LISTMAX     20
#define MAX(x, y) ((x) > (y) ? (x) : (y))
/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(size_t *)(p))
#define PUT(p, val)  (*(size_t *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* 插入 */
#define INSERT(bp, insert_bp) (*(size_t *)(bp) = (size_t)(insert_bp));

/* Address of free block's predecessor and successor entries */
#define PRED_BP(bp) ((char *)(bp))
#define SUCC_BP(bp) ((char *)(bp) + WSIZE)

/* Address of free block's predecessor and successor on the segregated list */
#define PRED(bp) (*(char **)(bp))
#define SUCC(bp) (*(char **)(SUCC_BP(bp)))
/* $end mallocmacros */

// 分离的空闲链表
void *LIST[LISTMAX];
//堆指针
static char *heapstart;

static void *extend_heap(size_t words);
static void *place(void *bp, size_t asize);
static void *coalesce(void *bp);
static void Insert(void *bp, size_t size);
static void Delete(void *bp);
int mm_init(void);
void *mm_malloc(size_t size);
void mm_free(void *ptr);
void *mm_realloc(void *ptr, size_t size);
int mm_check();

static void *extend_heap(size_t words)
{
	void *bp;
	size_t  size=ALIGN(words); 		//对齐
	if((bp=mem_sbrk(size)) == (void *)-1) return NULL;	//申请扩展的堆
	PUT(HDRP(bp), PACK(size, 0));	//设置头部
	PUT(FTRP(bp), PACK(size, 0));	//设置脚部
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));	//设置结尾
	Insert(bp, size);	// 把新建的堆插入到原堆中
	return coalesce(bp);	//返回合并后的堆
}

static void *place(void *bp, size_t asize)
{
	size_t csize=GET_SIZE(HDRP(bp));
	Delete(bp);
	if((csize-asize) < 2*DSIZE){	//如果剩余的大小小于最小块，则不分离原块
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
	}
	else if(asize >= 112){    //根据需要占用的空间大小选择分割方式
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));
        PUT(HDRP(NEXT_BLKP(bp)), PACK(asize, 1));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(asize, 1));
        Insert(bp, csize-asize);
        return NEXT_BLKP(bp);
    }
	else{
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		PUT(HDRP(NEXT_BLKP(bp)), PACK(csize-asize, 0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(csize-asize, 0));
		Insert(NEXT_BLKP(bp), csize-asize);
	}
	return bp;
}

static void *coalesce(void *bp)
{
	size_t size=GET_SIZE(HDRP(bp));
	size_t prev=GET_ALLOC(HDRP(PREV_BLKP(bp)));
	size_t next=GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	if(prev && next) return bp;			//前后都不空闲
	else if(!prev && next){			//前空闲后不空闲
		Delete(bp);
		Delete(PREV_BLKP(bp));
		size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
		bp = PREV_BLKP(bp);		//把bp指向块的起始即前驱的起始
	}
	else if(prev && !next){			//后空闲前不空闲
		Delete(bp);
		Delete(NEXT_BLKP(bp));
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size, 0));
	}
	else{							//前后均空闲
		Delete(bp);
		Delete(PREV_BLKP(bp));
		Delete(NEXT_BLKP(bp));
		size += GET_SIZE(HDRP(NEXT_BLKP(bp))) + GET_SIZE(HDRP(PREV_BLKP(bp)));
		PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
		PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
		bp = PREV_BLKP(bp);		//把bp指向块的起始即前驱的起始
	}
	Insert(bp, size);
	return bp;
}

static void Insert(void *bp, size_t size)
{
	int pos;
	void *pre_bp=NULL, *suc_bp=NULL;
	for(pos=0; pos<LISTMAX-1 && size>1; pos++) size>>=1;
	suc_bp = LIST[pos];
	while(suc_bp!=NULL && size>GET_SIZE(HDRP(suc_bp))){
		pre_bp = suc_bp;
		suc_bp = PRED(suc_bp);
	}
	if(pre_bp && suc_bp){
		INSERT(PRED_BP(bp), suc_bp);
		INSERT(SUCC_BP(suc_bp), bp);
		INSERT(SUCC_BP(bp), pre_bp);
		INSERT(PRED_BP(pre_bp), bp);
	}
	else if(pre_bp && !suc_bp){
		INSERT(PRED_BP(bp), suc_bp);
		INSERT(SUCC_BP(bp), pre_bp);
		INSERT(PRED_BP(pre_bp), bp);
	}
	else if(!pre_bp && suc_bp){
		INSERT(PRED_BP(bp), suc_bp);
		INSERT(SUCC_BP(suc_bp), bp);
		INSERT(SUCC_BP(bp), pre_bp);
		LIST[pos] = bp;
	}
	else{
		INSERT(PRED_BP(bp), suc_bp);
		INSERT(SUCC_BP(bp), pre_bp);
		LIST[pos] = bp;
	}
}

static void Delete(void *bp)
{
	int pos;
	size_t size=GET_SIZE(HDRP(bp));
	for(pos=0; pos<LISTMAX-1 && size>1; pos++) size>>=1;
	if(PRED(bp) && SUCC(bp)){
		INSERT(SUCC_BP(PRED(bp)), SUCC(bp));
		INSERT(PRED_BP(SUCC(bp)), PRED(bp));
	}
	else if(PRED(bp) && !SUCC(bp)){
		INSERT(SUCC_BP(PRED(bp)), SUCC(bp));
		LIST[pos] = PRED(bp);
	}
	else if(!PRED(bp) && SUCC(bp)){
		INSERT(PRED_BP(SUCC(bp)), PRED(bp));
	}
	else LIST[pos] = NULL;
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
	int pos;
	char *heap;
	for(pos=0; pos<LISTMAX; pos++) LIST[pos] = NULL;	//初始化链表
	if((heap=mem_sbrk(4*WSIZE)) == (void*)-1) return -1;	//申请堆
	PUT(heap, 0);						//对齐
	PUT(heap + WSIZE, PACK(DSIZE, 1));	//设置起始块的头部
	PUT(heap + 2*WSIZE, PACK(DSIZE, 1));//设置起始块的脚部
	PUT(heap + 3*WSIZE, PACK(0, 1));	//设置结束块的脚部
    heapstart = heap + DSIZE;
	if(extend_heap(CHUNKSIZE) == NULL) return -1;	//扩展堆
	//mm_check();	//检查一致性
    return 0;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
	size_t asize, extendsize, searchsize;
	int pos;
	void *bp=NULL;
	if(size == 0) return NULL;
	if(size <= DSIZE) asize = DSIZE + OVERHEAD;
	else asize = ALIGN(size+DSIZE);
	//寻找合适的空闲块
	searchsize = size;
	for(pos=0; pos < LISTMAX; pos++){
		if(searchsize <= 1 && (LIST[pos] != NULL)){
			bp = LIST[pos];
			while(bp != NULL && asize > GET_SIZE(HDRP(bp))) bp=PRED(bp);	//寻找和合适块
			if(bp != NULL) break;	//找到合适块
		}
		searchsize >>= 1;
	}
	if(bp == NULL){		//未找到
		extendsize = MAX(asize, CHUNKSIZE);
		if((bp=extend_heap(extendsize)) == NULL) return NULL;
	}

	bp = place(bp, asize);
	return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
	size_t size=GET_SIZE(HDRP(ptr));
	PUT(HDRP(ptr), PACK(size, 0));	//设置头部
	PUT(FTRP(ptr), PACK(size, 0));	//设置脚部
	Insert(ptr, size);	//插入到空闲链表
	coalesce(ptr);	//合并
	//mm_check();
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *newptr=ptr;
    size_t asize, remainsize, extendsize;
    if(size == 0){
    	mm_free(ptr);
    	return NULL;
	}
    if(size <= DSIZE) asize = DSIZE + OVERHEAD;		//内存对齐
	else asize = ALIGN(size + DSIZE);
    if(GET_SIZE(HDRP(ptr)) < asize){	//判断需要的大小是否比已有的大小大
    	//先检查连续地址的下一个块是否为空闲块或者该块是堆的结束块
    	if(!GET_SIZE(HDRP(NEXT_BLKP(ptr))) || !GET_ALLOC(HDRP(ptr))){
    		remainsize = GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr)));
    		if(remainsize < asize){	//不够的话扩展堆
    			extendsize = MAX(asize-remainsize, CHUNKSIZE);
    			if(extend_heap(extendsize) == NULL) return NULL;
				remainsize += extendsize;
			}
			//从空闲链表中删除刚刚利用的块并设置该块的头部和脚部
			Delete(NEXT_BLKP(ptr));
			PUT(HDRP(ptr), PACK(remainsize, 1));
			PUT(FTRP(ptr), PACK(remainsize, 1));
		}
		else{
			newptr = mm_malloc(asize);
			memcpy(newptr, ptr, GET_SIZE(HDRP(ptr)));
			mm_free(ptr);
		}
	}
	//mm_check();
	return newptr;
}

int mm_check(){
	void *search_bp, *cur_bp;
	int i, count=0;
	for(i=0; i<LISTMAX; i++){   //遍历每一条链
        search_bp = LIST[i];
        while(search_bp != NULL){   //遍历此链
            cur_bp = search_bp;
			if(GET_ALLOC(HDRP(cur_bp))){ //判断这个块是否都标识为空闲
                printf("warning:the list have a allocted block");
                return 0;
            }
            //能找到前后的块说明这个指针指向有效的空闲块,判断是否有连续的空闲块没有被合并
            if(!GET_ALLOC(HDRP(NEXT_BLKP(cur_bp))) || !GET_ALLOC(HDRP(PREV_BLKP(cur_bp)))) {
                printf("warning:some blocks are not coalesced");
                return 0;
            }
            count++;
            search_bp = PRED(search_bp);
        }
	}
    printf("pointer:%p", heapstart);
    search_bp = heapstart;
    while(GET_SIZE(HDRP(search_bp)) > 0){   //堆块中的指针是否指向有效的堆地址
        if(!GET_ALLOC(HDRP(search_bp))){
            count--;
        }
        search_bp = NEXT_BLKP(search_bp);
    }
    if(count != 0){   //判断是否每个空闲块都在空闲链表中
        puts("warning:not every free block is in the list");
        return 0;
   }
    return 1;
}
