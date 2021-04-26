#include "huffman.h"
HTNode *CreatHFM(FILE * fp, short *n, WeightType * FileLength)
{
	//文件指针，叶子数量，文件长度
	HTNode *ht = NULL;
	int i, m, s1, s2;
	MyType ch;

	ht = (HTNode *)malloc(2 * N * sizeof(HTNode));
	if (!ht)
		exit(1);

	for (i = 0; i < N; i++) {
		ht[i].weight = 0;
		ht[i].ch = (MyType)i;    /*1-n号ch 为字符，初始化 */
	}

	for (*FileLength = 0; !feof(fp); ++(*FileLength)) {
		ch = fgetc(fp);   //feof是C语言标准库函数，其功能是检测流上的文件结束符 
		ht[ch].weight++;   //当前字符，文件中查找到一个，权重加一 
	}
	--(*FileLength);    //去掉文件结束后的长度
	*n = SortTree(ht);//统计叶子数量
	m = *n * 2 - 1;   //每个二叉树有两个子节点，根节点只有一个，所以减一 
	
	if (1 == *n) {//如果只有一个字符
		ht[0].parent = 1;
		return ht;
	}
	else if (0 > *n)
		return NULL;

	for (i = m - 1; i >= 0; i--) {
		ht[i].LChild = 0;
		ht[i].parent = 0;
		ht[i].RChild = 0;
	}//倒序初始化节点指针
	/* ------------初始化完毕--------- */
	for (i = *n; i < m; i++)    //创建非叶子结点,建哈夫曼树
	{    //在ht[0]~ht[i-1]的范围内选择两个parent为0且weight最小的结点，其序号分别赋值给s1、s2返回
		SelectMinTree(ht, i - 1, &s1);
		ht[s1].parent = i;
		ht[i].LChild = s1;

		SelectMinTree(ht, i - 1, &s2);
		ht[s2].parent = i;
		ht[i].RChild = s2;

		ht[i].weight = ht[s1].weight + ht[s2].weight;
	}    /*哈夫曼树建立完毕 */

	return ht;
}
void SelectMinTree(HTNode * ht, int n, int *k)
{
	//查找最小节点
	int i, temp;
	WeightType min;

	
	for (i = 0; i <= n; i++) {
		if (0 == ht[i].parent) {
			min = ht[i].weight;    //init min
			temp = i;
			break;
		}
	}
	//冒泡找到最小节点
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
	return i;    //返回叶子个数
}
