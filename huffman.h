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
typedef unsigned char MyType;//重定义数据类型，避免后续更改麻烦
typedef struct  huffman//哈夫曼树
{
	MyType ch;    //存字符
	WeightType weight;    /* 用来存放各个结点的权值 */
	int parent, LChild, RChild;    /*指向双亲、孩子结点的指针 */
} HTNode;
typedef struct queue  //队列
{
	int tag;//标签
	int front;//队首
	int rear;//队尾
	MyType length;//队列长度
	char elem[Maxsize];//
} SeqQueue;
int InitQueue(SeqQueue * Q);//初始化创建队列函数
int In_seqQueue(SeqQueue * Q, char x);//入队函数
int Out_Queue(SeqQueue * Q, char *x);//出队函数
void SelectMinTree(HTNode * ht, int n, int *k);//查找树的最小节点
int SortTree(HTNode * ht);//霍夫曼树排序函数
char **CrtHuffmanCode(HTNode * ht, short LeafNum);//霍夫曼编码函数
HTNode *CreatHFM(FILE * fp, short *n, WeightType * FileLength);//创建霍夫曼树
MyType GetBits(SeqQueue * Q);//压缩函数之选取字符
void MaxMinLength(FILE * File, HTNode * ht, char **hc, short NLeaf, MyType * Max, MyType * Min);//求编码最长\最短长度
short CodeToFile(FILE * fp, char **hc, short n, SeqQueue * Q, MyType * length);//压缩写入函数
void Compress();//压缩主函数
void ToQueue(SeqQueue * Q, MyType ch);//字符入队函数
void UnCompress();//解压主函数
