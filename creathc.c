#include "huffman.h"
//Çó¹þ·òÂü±àÂë±í
char **CrtHuffmanCode(HTNode * ht, short LeafNum)
{
	char *cd, **hc;    
	int i, start, p, last;

	hc = (char **)malloc((LeafNum) * sizeof(char *));    

	if (1 == LeafNum)    
	{
		hc[0] = (char *)malloc((LeafNum + 1) * sizeof(char));
		strcpy(hc[0], "0");
		return hc;
	}

	cd = (char *)malloc((LeafNum + 1) * sizeof(char));    
	cd[LeafNum] = '\0';    
	for (i = 0; i < LeafNum; i++) {    
		start = LeafNum;    
		last = i;
		for (p = ht[i].parent; p != 0; p = ht[p].parent) {    
			if (ht[p].LChild == last)
				cd[--start] = '0';   
			else
				cd[--start] = '1';    
			last = p;
		}
		hc[i] = (char *)malloc((LeafNum - start) * sizeof(char));    
		strcpy(hc[i], &cd[start]);
		printf("%3dºÅ %3c Âë³¤:%2d;±àÂë:%s\n", ht[i].ch, ht[i].ch, LeafNum - start, &cd[start]);
	}   
	free(cd);    
	return hc;
}
