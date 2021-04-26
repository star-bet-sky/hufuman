#include "huffman.h"
int InitQueue(SeqQueue * Q)
{
	if (!Q)
		return 1;
	Q->tag = Empty;
	Q->front = Q->rear = 0;
	Q->length = 0;

	return 0;
}

int In_seqQueue(SeqQueue * Q, char x)
{
	if (Q->front == Q->rear && Q->tag == SOME)
		return FULL;    //full队满条件

	Q->elem[Q->rear] = x;   
	Q->rear = (Q->rear + 1) % Maxsize;
	Q->length++;
	Q->tag = SOME;
	return SOME;
}
int Out_Queue(SeqQueue * Q, char *x)
{
	if (Q->tag == Empty)
		return Empty;

	*x = Q->elem[Q->front];
	Q->length--;
	Q->front = (Q->front + 1) % Maxsize;

	if (Q->front == Q->rear)
		Q->tag = Empty;

	return SOME;
}