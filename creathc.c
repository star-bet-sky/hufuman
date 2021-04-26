#include "huffman.h"
//求哈夫曼0-1字符编码表
char **CrtHuffmanCode(HTNode * ht, short LeafNum)
/*从叶子结点到根，逆向求每个叶子结点对应的哈夫曼编码*/
{
	char *cd, **hc;    //容器，huffman编码存储结构
	int i, start, p, last;

	hc = (char **)malloc((LeafNum) * sizeof(char *));    /*分配n个编码的头指针 */

	if (1 == LeafNum)    //只有一个叶子节点时 
	{
		hc[0] = (char *)malloc((LeafNum + 1) * sizeof(char));
		strcpy(hc[0], "0");
		return hc;
	}

	cd = (char *)malloc((LeafNum + 1) * sizeof(char));    /*分配求当前编码的工作空间 */
	cd[LeafNum] = '\0';    /*从右向左逐位存放编码，首先存放编码结束符 */
	for (i = 0; i < LeafNum; i++) {    /*求n个叶子结点对应的哈夫曼编码 */
		start = LeafNum;    /*初始化编码起始指针 */
		last = i;
		for (p = ht[i].parent; p != 0; p = ht[p].parent) {    /*从叶子到根结点求编码 */
			if (ht[p].LChild == last)
				cd[--start] = '0';    /*左分支标0 */
			else
				cd[--start] = '1';    /*右分支标1 */
			last = p;
		}
		hc[i] = (char *)malloc((LeafNum - start) * sizeof(char));    /*为第i个编码分配空间 */
		strcpy(hc[i], &cd[start]);
		//
		printf("%3d号 %3c 码长:%2d;编码:%s\n", ht[i].ch, ht[i].ch, LeafNum - start, &cd[start]);
	}   
	free(cd);    
	return hc;
}
