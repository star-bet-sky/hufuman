#include "huffman.h"
//解压缩
void UnCompress()
{
	MyType *str, MaxLength, MinLength, ch, *num, finalLength = 0, final = 0;
	FILE *cf, *uf;
	short NLeaf, Ncom;
	char CName[N], UName[N], **hc, *p, x, *buf;
	SeqQueue *Q = NULL;
	int i, j;
	WeightType srcLen = 0, thisFile = 0;

	fflush(stdin);    //清空输入缓冲区域
	printf("解压文件名：");
	scanf("%s", CName);
	if (CName[0] == '\0') {
		printf("try again:");
		scanf("%s", CName);
		fflush(stdin);
	}
	fflush(stdin);    //清空输入缓冲区域
	printf("解压后文件名：");
	scanf("%s", UName);
	if (UName[0] == '\0'{
		printf("在输入一次:");
		scanf("%s", UName);
	}

	cf = fopen(CName, "rb");
	uf = fopen(UName, "wb");
	if (!cf || !uf) {
		puts("无法打开文件");
		return;
	}

	fread(&srcLen, sizeof(WeightType), 1, cf);    
	fread(&finalLength, 1, 1, cf);    
	fread(&NLeaf, sizeof(short), 1, cf);    
	fread(&MaxLength, sizeof(MyType), 1, cf);
	fread(&MinLength, sizeof(MyType), 1, cf);
	//printf("MaxLength = %d;minLen= %d,Leaf= %d\n",MaxLength,MinLength,NLeaf);getchar();
	Q = (SeqQueue *)malloc(sizeof(SeqQueue));

	buf = (char *)malloc((2 + MaxLength * sizeof(char)));
	str = (MyType *)malloc(NLeaf * sizeof(MyType));
	num = (MyType *)malloc(NLeaf * sizeof(MyType));
	//压缩叶子数量x和最后剩长
	if (!Q || !str || !num || !buf) {
		puts("Memery error.");
		exit(1);
	}
	InitQueue(Q);    //初始化

	fread(&Ncom, sizeof(short), 1, cf);
	fread(&final, sizeof(MyType), 1, cf);

	for (i = 0; i < NLeaf; i++)    //读取叶子及其码长
	{
		fread(&str[i], sizeof(MyType), 1, cf);
		fread(&num[i], sizeof(MyType), 1, cf);
		//printf("chars= %c;num= %d\n",Chars[i],CHlength[i]);
	}
	
	

	hc = (char **)malloc((NLeaf) * sizeof(char *));    //hc为编码表的指针数组
	if (!hc)
		exit(1);

	--Ncom;   
	for (j = i = 0; i < Ncom; i++) {
		ch = fgetc(cf);   
		ToQueue(Q, ch);    
		while (Q->length > MaxLength) {
			hc[j] = p = (char *)malloc(1 + num[j]);
			for (ch = 0; ch < num[j]; ch++) {
				Out_Queue(Q, &x);
				*p++ = x;
			}
			*p = '\0';    
			j++;
		}
	}
	ch = fgetc(cf);    //最后一个
	ToQueue(Q, ch);
	final = 8 - final;
	while (Q->length > final) {
		p = hc[j] = (char *)malloc(1 + num[j]);
		for (ch = 0; ch < num[j]; ch++) {
			Out_Queue(Q, &x);
			*p++ = x;
		}
		*p = '\0';   
		j++;
	}

	InitQueue(Q);
	--srcLen;
	--MinLength;
	
	printf("Please wait a minute,uncompressing...");
	while (thisFile < srcLen) {
		
		ch = fgetc(cf);    
		ToQueue(Q, ch);
		thisFile++;    
		//完了后队列长于码串的最大值

		while (Q->length > MaxLength) {
			for (i = 0; i < MinLength; i++) {
				Out_Queue(Q, &x);
				buf[i] = x;
			}
			for (; i < MaxLength; i++) {
				Out_Queue(Q, &x);
				buf[i] = x;
				buf[i + 1] = '\0';
				for (j = 0; j < NLeaf; j++) {
					if (i + 1 == num[j]
						&& 0 == strcmp(hc[j], buf)) {
						ch = str[j];   
						fputc(ch, uf);   
						break;
					}
				}
				if (j < NLeaf)
					break;
			}
		}    //while MaxLength
	}

	ch = fgetc(cf);   //读取最后一个压缩字
	ToQueue(Q, ch);

	finalLength = 8 - finalLength;
	while (Q->length > finalLength) {
		for (i = 0; i < MinLength; i++) {
			Out_Queue(Q, &x);
			buf[i] = x;
		}
		for (; i < MaxLength; i++) {
			Out_Queue(Q, &x);
			buf[i] = x;
			buf[i + 1] = '\0';
			for (j = 0; j < NLeaf; j++) {
				if (i + 1 == num[j] && 0 == strcmp(hc[j], buf)) {
					ch = str[j];   
					fputc(ch, uf);    
					break;
				}
			}
			if (j < NLeaf)
				break;
		}
	}
	free(Q);
	free(str);
	free(num);
	free(hc);
	fclose(uf);
	fclose(cf);
}
//从队列里取8个字符（0、1），转换成一个字节
void ToQueue(SeqQueue * Q, MyType ch)
{
	int i;
	MyType temp;

	for (i = 0; i < 8; i++) {
		temp = ch << i;
		temp = temp >> 7;
		if (1 == temp)
			In_seqQueue(Q, '1');    //printf("1");
		else
			In_seqQueue(Q, '0');    //printf("0");
	}    //puts("");
}
