#include "huffman.h"
//��ѹ��
void UnCompress()
{
	MyType *str, MaxLength, MinLength, ch, *num, finalLength = 0, final = 0;
	FILE *beferfile, *afterfile;
	short NLeaf, Ncom;
	char befercom[N], aftercom[N], **hc, *p, x, *buf;
	SeqQueue *Q = NULL;
	int i, j;
	WeightType srcLen = 0, thisFile = 0;

	fflush(stdin);    
	printf("��ѹ�ļ�����");
	scanf("%s", befercom);
	if (befercom[0] == '\0') {
		printf("try again:");
		scanf("%s", befercom);
		fflush(stdin);
	}
	fflush(stdin);   
	printf("��ѹ���ļ�����");
	scanf("%s", aftercom);
	if (aftercom[0] == '\0') {
		printf("������һ��:");
		scanf("%s", aftercom);
	}

	beferfile = fopen(befercom, "rb");
	afterfile = fopen(aftercom, "wb");
	if (!beferfile || !afterfile) {
		puts("�޷����ļ�");
		return;
	}

	fread(&srcLen, sizeof(WeightType), 1, beferfile);    
	fread(&finalLength, 1, 1, beferfile);    
	fread(&NLeaf, sizeof(short), 1, beferfile);    
	fread(&MaxLength, sizeof(MyType), 1, beferfile);
	fread(&MinLength, sizeof(MyType), 1, beferfile);
	Q = (SeqQueue *)malloc(sizeof(SeqQueue));

	buf = (char *)malloc((2 + MaxLength * sizeof(char)));
	str = (MyType *)malloc(NLeaf * sizeof(MyType));
	num = (MyType *)malloc(NLeaf * sizeof(MyType));
	if (!Q || !str || !num || !buf) {
		puts("�ռ�������");
		exit(1);
	}
	InitQueue(Q);   

	fread(&Ncom, sizeof(short), 1, beferfile);
	fread(&final, sizeof(MyType), 1, beferfile);

	for (i = 0; i < NLeaf; i++)   
	{
		fread(&str[i], sizeof(MyType), 1, beferfile);
		fread(&num[i], sizeof(MyType), 1, beferfile);
	}
	hc = (char **)malloc((NLeaf) * sizeof(char *));    
	if (!hc)
		exit(1);

	--Ncom;   
	for (j = i = 0; i < Ncom; i++) {
		ch = fgetc(beferfile);   
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
	ch = fgetc(beferfile);    
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
	
	printf("���ڽ�ѹ");
	while (thisFile < srcLen) {
		
		ch = fgetc(beferfile);    
		ToQueue(Q, ch);
		thisFile++;    

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
						fputc(ch, afterfile);   
						break;
					}
				}
				if (j < NLeaf)
					break;
			}
		}   
	}

	ch = fgetc(beferfile);  
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
					fputc(ch, afterfile);    
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
	fclose(afterfile);
	fclose(beferfile);
}
//�Ӷ�����ȡ8���ַ���ת����һ���ֽ�
void ToQueue(SeqQueue * Q, MyType ch)
{
	int i;
	MyType temp;

	for (i = 0; i < 8; i++) {
		temp = ch << i;
		temp = temp >> 7;
		if (1 == temp)
			In_seqQueue(Q, '1');   
		else
			In_seqQueue(Q, '0');    
	}   
}
