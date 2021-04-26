#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define N 256
#define Maxsize 80 //����������󳤶�
#define SOME 1
#define Empty 0
#define FULL -1

typedef unsigned long int WeightType;
typedef unsigned char MyType;//�ض����������ͣ�������������鷳
typedef struct  huffman//��������
{
	MyType ch;    //���ַ�
	WeightType weight;    /* ������Ÿ�������Ȩֵ */
	int parent, LChild, RChild;    /*ָ��˫�ס����ӽ���ָ�� */
} HTNode;
typedef struct queue  //����
{
	int tag;//��ǩ
	int front;//����
	int rear;//��β
	MyType length;//���г���
	char elem[Maxsize];//
} SeqQueue;
int InitQueue(SeqQueue * Q);//��ʼ���������к���
int In_seqQueue(SeqQueue * Q, char x);//��Ӻ���
int Out_Queue(SeqQueue * Q, char *x);//���Ӻ���
void SelectMinTree(HTNode * ht, int n, int *k);//����������С�ڵ�
int SortTree(HTNode * ht);//��������������
char **CrtHuffmanCode(HTNode * ht, short LeafNum);//���������뺯��
HTNode *CreatHFM(FILE * fp, short *n, WeightType * FileLength);//������������
MyType GetBits(SeqQueue * Q);//ѹ������֮ѡȡ�ַ�
void MaxMinLength(FILE * File, HTNode * ht, char **hc, short NLeaf, MyType * Max, MyType * Min);//������\��̳���
short CodeToFile(FILE * fp, char **hc, short n, SeqQueue * Q, MyType * length);//ѹ��д�뺯��
void Compress();//ѹ��������
void ToQueue(SeqQueue * Q, MyType ch);//�ַ���Ӻ���
void UnCompress();//��ѹ������
