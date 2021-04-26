#include <Windows.h>
#include <assert.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define N 256
#define Maxsize 80 //霍夫曼树最大长度
#define SOME 1
#define Empty 0
#define FULL -1

typedef unsigned long int WeightType;
typedef unsigned char MyType;
typedef struct  huffman//哈夫曼树结构体
{
	MyType ch;    //存字符
	WeightType weight;    //结点权值 
	int parent, LChild, RChild;    //指向双亲、孩子结点的指针
} HTNode;
typedef struct queue  //队列
{
	int tag;//标签
	int front;//队首
	int rear;//队尾
	MyType length;//队列长度
	char elem[Maxsize];//存放字符元素
} SeqQueue;
int InitQueue(SeqQueue * Q);
int In_seqQueue(SeqQueue * Q, char x);
int Out_Queue(SeqQueue * Q, char *x);
void SelectMinTree(HTNode * ht, int n, int *k);
int SortTree(HTNode * ht);
char **CrtHuffmanCode(HTNode * ht, short LeafNum);
HTNode *CreatHFM(FILE * fp, short *n, WeightType * FileLength);
MyType GetBits(SeqQueue * Q);
void MaxMinLength(FILE * File, HTNode * ht, char **hc, short NLeaf, MyType * Max, MyType * Min);
short CodeToFile(FILE * fp, char **hc, short n, SeqQueue * Q, MyType * length);
void ToQueue(SeqQueue * Q, MyType ch);
void UnCompress();
void Compress();
