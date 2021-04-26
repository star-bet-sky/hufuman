#include "huffman.h"
//压缩
void Compress()
{
	char desFile[80], rename[80];
	MyType maxLen, minLen, ch, bits, n, finalLength;
	int i;
	short LeafNum, codeNum;//叶子数和编码数
	WeightType count = 0, Length = 0, FileLength;
	FILE *fp, *compressFile;
	SeqQueue *Q;
	HTNode *ht = NULL;
	char **hc = NULL, **Map = NULL, *p;

	printf("需要压缩的文件名:");
	scanf("%s", desFile);

	printf("压缩后文件名:");
	scanf("%s", rename);
	if ('\0' == rename[0]) {
		printf("输入不能为空，请重新输入:");
		scanf("%s", rename);
	}

	compressFile = fopen(rename, "wb");//创建一个压缩后的文件
	fp = fopen(desFile, "rb");    //原文件

	if (!fp || !compressFile) {
		puts("无法打开文件");
		return;
	}

	ht = CreatHFM(fp, &LeafNum, &FileLength);    //创建哈夫曼树,统计叶子个数和原文件长度
	if (!FileLength) {    
		printf("文件为空，无须压缩...");
		fclose(fp);
		fclose(compressFile);
		free(ht);
		return;
	}
	Q = (SeqQueue *)malloc(sizeof(SeqQueue));//创建一个压缩队列空间
	InitQueue(Q);   //创建一个队列
	hc = CrtHuffmanCode(ht, LeafNum);    //取得哈夫曼0、1编码,hc的长度为LeafNum
	//Map二维数组为了取编码好定位，再建立全部//
	Map = (char **)malloc(N * sizeof(char *));    //字符编码表
	if (!Map) {
		puts("申请空间失败");
		return;
	}

	for (i = 0; i < N; i++)    //初始化
		Map[i] = NULL;

	for (i = 0; i < LeafNum; i++)    // 定位，编码指针数组Map[256]
		Map[(int)(ht[i].ch)] = hc[i];

	fseek(compressFile, sizeof(WeightType) + sizeof(short) + 6 * sizeof(MyType), SEEK_SET);
	//fseek：设置文件指针stream的位置，从文件开头位置偏移sizeof(WeightType) + sizeof(short) + 6 * sizeof(MyType)多
	//先占个位置,等下填压缩叶子编码剩几个和最长编码长
	//SEEK_SET:文件开头 SEEK_CUR:当前位置 SEEK_END:文件结尾
	MaxMinLength(compressFile, ht, hc, LeafNum, &maxLen, &minLen);    //获得最长码串长度,顺便填写字符对应编码长
	free(ht);    
	codeNum = CodeToFile(compressFile, hc, LeafNum, Q, &finalLength);    //把字符转成其二进制编码写入文件,返回压成多少个

	rewind(compressFile);    //使文件指针移到开始
	
	fseek(compressFile, sizeof(WeightType) + sizeof(MyType), SEEK_SET);
	fwrite(&LeafNum, sizeof(short), 1, compressFile);    //写入叶子个数
	fwrite(&maxLen, sizeof(MyType), 1, compressFile);    //最长码串长度
	fwrite(&minLen, sizeof(MyType), 1, compressFile);    //最短码串长度
	
	fwrite(&codeNum, sizeof(short), 1, compressFile);    //填写叶子编码压多少个
	fwrite(&finalLength, sizeof(MyType), 1, compressFile);    //最后剩

	fseek(compressFile, 2 * LeafNum * sizeof(MyType) + codeNum, SEEK_CUR);
	fseek(fp, 0, SEEK_SET);
	printf("正在压缩");

	while (count < FileLength) {//压缩循环
		ch = fgetc(fp);    
		++count;    
		for (p = Map[ch]; *p != '\0'; p++)
			In_seqQueue(Q, *p);

		while (Q->length > 8)    
		{
			bits = GetBits(Q);    //出队8个元素,合成一个字节
			fputc(bits, compressFile);     
			Length++;    
		}
	}    //循环结束
	//最后一个bits ;
	finalLength = Q->length;    
	n = 8 - finalLength;
	bits = GetBits(Q);
	for (i = 0; i < n; i++)
		bits = bits << 1;    //以‘0’补
		//fputc(bits,compressFile);
	fwrite(&bits, sizeof(MyType), 1, compressFile);    //printf("\nfinal压：%c\n",bits);
	Length++;

	rewind(compressFile);    
	fwrite(&Length, sizeof(WeightType), 1, compressFile);    //压缩后的长
	fwrite(&finalLength, sizeof(char), 1, compressFile);    //最后的串长

	Length = Length + 12 + codeNum;   

	if (Length >= FileLength)
		puts("\n压缩率: 0.0%");
	else
		printf("\n压缩率: %.2lf%c\n", (double)((FileLength - Length) / (double)FileLength) * 100.0, '%');//将源文件长度和压缩后长度转化为double类型进行压缩率计算
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
		fwrite(&ht[i].ch, sizeof(MyType), 1, File);    //字符和对应的
		fwrite(&length, sizeof(MyType), 1, File);    //编码长度写进文件
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

	for (i = 0; i < n; i++)    // 将n个叶子压缩并写入文件
	{
		for (p = hc[i]; '\0' != *p; p++)
			In_seqQueue(Q, *p);

		while (Q->length > 8) {    // puts("出队");
			bits = GetBits(Q);    //出队8个元素
			fputc(bits, fp);    //fwrite(&bits,sizeof(char),1,fp);
			//printf("压字符 %c\n",bits);
			count++;
		}
	}    // printf("码字最后剩: %d\n",Q->length);

	*length = Q->length;
	i = 8 - *length;
	bits = GetBits(Q);    //取8个如果队不空
	for (j = 0; j < i; j++)
		bits = bits << 1;    //printf("压 字符 %c\n",bits);
	fputc(bits, fp);    //fwrite(&bits,sizeof(char),1,fp);
	count++;    //printf(" 指 针 在%d \n",ftell(fp));

	InitQueue(Q);
	return count;
}

