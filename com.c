#include "huffman.h"
//压缩
void Compress()
{
	char befername[80], aftername[80];
	MyType maxLen, minLen, ch, bits, n, finalLength;
	int i;
	short LeafNum, codeNum;
	WeightType count = 0, Length = 0, FileLength;
	FILE *fp, *compressFile;
	SeqQueue *Q;
	HTNode *ht = NULL;
	char **hc = NULL, **Map = NULL, *p;

	printf("需要压缩的文件名:");
	scanf("%s", befername);

	printf("压缩后文件名:");
	scanf("%s", aftername);
	if ('\0' == aftername[0]) {
		printf("输入不能为空，请重新输入:");
		scanf("%s", aftername);
	}

	compressFile = fopen(aftername, "wb");
	fp = fopen(befername, "rb");    

	if (!fp || !compressFile) {
		puts("无法打开文件");
		return;
	}

	ht = CreatHFM(fp, &LeafNum, &FileLength);    
	if (!FileLength) {    
		printf("文件为空，无须压缩...");
		fclose(fp);
		fclose(compressFile);
		free(ht);
		return;
	}
	Q = (SeqQueue *)malloc(sizeof(SeqQueue));
	InitQueue(Q);   
	hc = CrtHuffmanCode(ht, LeafNum);    
	Map = (char **)malloc(N * sizeof(char *)); 
	if (!Map) {
		puts("申请空间失败");
		return;
	}

	for (i = 0; i < N; i++)  
		Map[i] = NULL;//初始化map

	for (i = 0; i < LeafNum; i++)    
		Map[(int)(ht[i].ch)] = hc[i];

	fseek(compressFile, sizeof(WeightType) + sizeof(short) + 6 * sizeof(MyType), SEEK_SET);
	MaxMinLength(compressFile, ht, hc, LeafNum, &maxLen, &minLen);   


	free(ht);    
	codeNum = CodeToFile(compressFile, hc, LeafNum, Q, &finalLength);    
	rewind(compressFile);    
	
	fseek(compressFile, sizeof(WeightType) + sizeof(MyType), SEEK_SET);
	fwrite(&LeafNum, sizeof(short), 1, compressFile);    
	fwrite(&maxLen, sizeof(MyType), 1, compressFile);    
	fwrite(&minLen, sizeof(MyType), 1, compressFile);    
	
	fwrite(&codeNum, sizeof(short), 1, compressFile);    
	fwrite(&finalLength, sizeof(MyType), 1, compressFile);    

	fseek(compressFile, 2 * LeafNum * sizeof(MyType) + codeNum, SEEK_CUR);
	fseek(fp, 0, SEEK_SET);
	printf("正在压缩");

	while (count < FileLength) {
		ch = fgetc(fp);    
		++count;    
		for (p = Map[ch]; *p != '\0'; p++)
			In_seqQueue(Q, *p);

		while (Q->length > 8)    
		{
			bits = GetBits(Q);    
			fputc(bits, compressFile);     
			Length++;    
		}
	}   
	finalLength = Q->length;    
	n = 8 - finalLength;
	bits = GetBits(Q);
	for (i = 0; i < n; i++)
		bits = bits << 1;    
	fwrite(&bits, sizeof(MyType), 1, compressFile);   

	rewind(compressFile);    
	fwrite(&Length, sizeof(WeightType), 1, compressFile);    
	fwrite(&finalLength, sizeof(char), 1, compressFile);   

	Length = Length + 12 + codeNum;   

	if (Length >= FileLength)
		puts("\n压缩率: 0.0%");
	else
		printf("\n压缩率: %.2lf%c\n", (double)((FileLength - Length) / (double)FileLength) * 100.0, '%');
	fclose(fp);
	fclose(compressFile);
	free(Q);
	free(hc);
	free(Map);
}
//把读出的字符，转换成8个0、1字符串并入队
MyType GetBits(SeqQueue * Q)
{
	MyType i, bits = 0;
	char x;

	for (i = 0; i < 8; i++) {
		if (Out_Queue(Q, &x) != Empty) {    
			if ('0' == x)
				bits = bits << 1;
			else
				bits = (bits << 1) | 1;
		}
		else
			break;
	}  

	return bits;
}

//求最长（最短）编码长度
void MaxMinLength(FILE * File, HTNode * ht, char **hc, short NLeaf, MyType * Max, MyType * Min)
{
	int i;
	MyType length;

	*Max = *Min = strlen(hc[0]);
	for (i = 0; i < NLeaf; i++) {
		length = strlen(hc[i]);
		fwrite(&ht[i].ch, sizeof(MyType), 1, File);    
		fwrite(&length, sizeof(MyType), 1, File);   
		if (length > *Max)
			*Max = length;
		if (length < *Min)
			*Min = length;
	}
}
//把出现过的字符编码表经过压缩写进文件
short CodeToFile(FILE * fp, char **hc, short n, SeqQueue * Q, MyType * length)
{
	int i;
	char *p;
	MyType j, bits;
	short count = 0;

	for (i = 0; i < n; i++)   
	{
		for (p = hc[i]; '\0' != *p; p++)
			In_seqQueue(Q, *p);

		while (Q->length > 8) {    
			bits = GetBits(Q);    
			fputc(bits, fp);    
			count++;
		}
	}  

	*length = Q->length;
	i = 8 - *length;
	bits = GetBits(Q);    
	for (j = 0; j < i; j++)
		bits = bits << 1;   ;
	fputc(bits, fp);   
	count++;   

	InitQueue(Q);
	return count;
}

