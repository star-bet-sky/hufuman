#include "huffman.h"
HTNode *CreatHFM(FILE * fp, short *n, WeightType * FileLength)
{
	HTNode *ht = NULL;
	int i, m, s1, s2;
	MyType ch;

	ht = (HTNode *)malloc(2 * N * sizeof(HTNode));
	if (!ht)
		exit(1);

	for (i = 0; i < N; i++) {
		ht[i].weight = 0;
		ht[i].ch = (MyType)i;    
	}

	for (*FileLength = 0; !feof(fp); ++(*FileLength)) {
		ch = fgetc(fp);    
		ht[ch].weight++;    
	}
	--(*FileLength);   
	*n = SortTree(ht);
	m = *n * 2 - 1;    
	
	if (1 == *n) {
		ht[0].parent = 1;
		return ht;
	}
	else if (0 > *n)
		return NULL;

	for (i = m - 1; i >= 0; i--) {
		ht[i].LChild = 0;
		ht[i].parent = 0;
		ht[i].RChild = 0;
	}
	for (i = *n; i < m; i++)    
	{    
		SelectMinTree(ht, i - 1, &s1);
		ht[s1].parent = i;
		ht[i].LChild = s1;

		SelectMinTree(ht, i - 1, &s2);
		ht[s2].parent = i;
		ht[i].RChild = s2;

		ht[i].weight = ht[s1].weight + ht[s2].weight;
	}

	return ht;
}
//查找最小节点
void SelectMinTree(HTNode * ht, int n, int *k)
{
	int i, temp;
	WeightType min;

	
	for (i = 0; i <= n; i++) {
		if (0 == ht[i].parent) {
			min = ht[i].weight;    //init min
			temp = i;
			break;
		}
	}
	for (i++; i <= n; i++) {
		if (0 == ht[i].parent && ht[i].weight < min) {
			min = ht[i].weight;
			temp = i;
		}
	}
	*k = temp;
}
// 对哈夫曼树排序，并统计叶子数量
int SortTree(HTNode * ht)
{
	short i, j;
	HTNode tmp;

	for (i = N - 1; i >= 0; i--) {
		for (j = 0; j < i; j++)
			if (ht[j].weight < ht[j + 1].weight) {
				tmp = ht[j];
				ht[j] = ht[j + 1];
				ht[j + 1] = tmp;
			}
	}
	for (i = 0; i < N; i++)
		if (0 == ht[i].weight)
			return i;
	return i;    
}
