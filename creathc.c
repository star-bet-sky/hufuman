#include "huffman.h"
//�������0-1�ַ������
char **CrtHuffmanCode(HTNode * ht, short LeafNum)
/*��Ҷ�ӽ�㵽����������ÿ��Ҷ�ӽ���Ӧ�Ĺ���������*/
{
	char *cd, **hc;    //������huffman����洢�ṹ
	int i, start, p, last;

	hc = (char **)malloc((LeafNum) * sizeof(char *));    /*����n�������ͷָ�� */

	if (1 == LeafNum)    //ֻ��һ��Ҷ�ӽڵ�ʱ 
	{
		hc[0] = (char *)malloc((LeafNum + 1) * sizeof(char));
		strcpy(hc[0], "0");
		return hc;
	}

	cd = (char *)malloc((LeafNum + 1) * sizeof(char));    /*������ǰ����Ĺ����ռ� */
	cd[LeafNum] = '\0';    /*����������λ��ű��룬���ȴ�ű�������� */
	for (i = 0; i < LeafNum; i++) {    /*��n��Ҷ�ӽ���Ӧ�Ĺ��������� */
		start = LeafNum;    /*��ʼ��������ʼָ�� */
		last = i;
		for (p = ht[i].parent; p != 0; p = ht[p].parent) {    /*��Ҷ�ӵ����������� */
			if (ht[p].LChild == last)
				cd[--start] = '0';    /*���֧��0 */
			else
				cd[--start] = '1';    /*�ҷ�֧��1 */
			last = p;
		}
		hc[i] = (char *)malloc((LeafNum - start) * sizeof(char));    /*Ϊ��i���������ռ� */
		strcpy(hc[i], &cd[start]);
		//
		printf("%3d�� %3c �볤:%2d;����:%s\n", ht[i].ch, ht[i].ch, LeafNum - start, &cd[start]);
	}   
	free(cd);    
	return hc;
}
