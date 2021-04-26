#include "huffman.h"
//ѹ��
void Compress()
{
	char desFile[80], rename[80];
	MyType maxLen, minLen, ch, bits, n, finalLength;
	int i;
	short LeafNum, codeNum;//Ҷ�����ͱ�����
	WeightType count = 0, Length = 0, FileLength;
	FILE *fp, *compressFile;
	SeqQueue *Q;
	HTNode *ht = NULL;
	char **hc = NULL, **Map = NULL, *p;

	printf("��Ҫѹ�����ļ���:");
	scanf("%s", desFile);

	printf("ѹ�����ļ���:");
	scanf("%s", rename);
	if ('\0' == rename[0]) {
		printf("���벻��Ϊ�գ�����������:");
		scanf("%s", rename);
	}

	compressFile = fopen(rename, "wb");//����һ��ѹ������ļ�
	fp = fopen(desFile, "rb");    //ԭ�ļ�

	if (!fp || !compressFile) {
		puts("�޷����ļ�");
		return;
	}

	ht = CreatHFM(fp, &LeafNum, &FileLength);    //������������,ͳ��Ҷ�Ӹ�����ԭ�ļ�����
	if (!FileLength) {    
		printf("�ļ�Ϊ�գ�����ѹ��...");
		fclose(fp);
		fclose(compressFile);
		free(ht);
		return;
	}
	Q = (SeqQueue *)malloc(sizeof(SeqQueue));//����һ��ѹ�����пռ�
	InitQueue(Q);   //����һ������
	hc = CrtHuffmanCode(ht, LeafNum);    //ȡ�ù�����0��1����,hc�ĳ���ΪLeafNum
	//Map��ά����Ϊ��ȡ����ö�λ���ٽ���ȫ��//
	Map = (char **)malloc(N * sizeof(char *));    //�ַ������
	if (!Map) {
		puts("����ռ�ʧ��");
		return;
	}

	for (i = 0; i < N; i++)    //��ʼ��
		Map[i] = NULL;

	for (i = 0; i < LeafNum; i++)    // ��λ������ָ������Map[256]
		Map[(int)(ht[i].ch)] = hc[i];

	fseek(compressFile, sizeof(WeightType) + sizeof(short) + 6 * sizeof(MyType), SEEK_SET);
	//fseek�������ļ�ָ��stream��λ�ã����ļ���ͷλ��ƫ��sizeof(WeightType) + sizeof(short) + 6 * sizeof(MyType)��
	//��ռ��λ��,������ѹ��Ҷ�ӱ���ʣ����������볤
	//SEEK_SET:�ļ���ͷ SEEK_CUR:��ǰλ�� SEEK_END:�ļ���β
	MaxMinLength(compressFile, ht, hc, LeafNum, &maxLen, &minLen);    //�����봮����,˳����д�ַ���Ӧ���볤
	free(ht);    
	codeNum = CodeToFile(compressFile, hc, LeafNum, Q, &finalLength);    //���ַ�ת��������Ʊ���д���ļ�,����ѹ�ɶ��ٸ�

	rewind(compressFile);    //ʹ�ļ�ָ���Ƶ���ʼ
	
	fseek(compressFile, sizeof(WeightType) + sizeof(MyType), SEEK_SET);
	fwrite(&LeafNum, sizeof(short), 1, compressFile);    //д��Ҷ�Ӹ���
	fwrite(&maxLen, sizeof(MyType), 1, compressFile);    //��봮����
	fwrite(&minLen, sizeof(MyType), 1, compressFile);    //����봮����
	
	fwrite(&codeNum, sizeof(short), 1, compressFile);    //��дҶ�ӱ���ѹ���ٸ�
	fwrite(&finalLength, sizeof(MyType), 1, compressFile);    //���ʣ

	fseek(compressFile, 2 * LeafNum * sizeof(MyType) + codeNum, SEEK_CUR);
	fseek(fp, 0, SEEK_SET);
	printf("����ѹ��");

	while (count < FileLength) {//ѹ��ѭ��
		ch = fgetc(fp);    
		++count;    
		for (p = Map[ch]; *p != '\0'; p++)
			In_seqQueue(Q, *p);

		while (Q->length > 8)    
		{
			bits = GetBits(Q);    //����8��Ԫ��,�ϳ�һ���ֽ�
			fputc(bits, compressFile);     
			Length++;    
		}
	}    //ѭ������
	//���һ��bits ;
	finalLength = Q->length;    
	n = 8 - finalLength;
	bits = GetBits(Q);
	for (i = 0; i < n; i++)
		bits = bits << 1;    //�ԡ�0����
		//fputc(bits,compressFile);
	fwrite(&bits, sizeof(MyType), 1, compressFile);    //printf("\nfinalѹ��%c\n",bits);
	Length++;

	rewind(compressFile);    
	fwrite(&Length, sizeof(WeightType), 1, compressFile);    //ѹ����ĳ�
	fwrite(&finalLength, sizeof(char), 1, compressFile);    //���Ĵ���

	Length = Length + 12 + codeNum;   

	if (Length >= FileLength)
		puts("\nѹ����: 0.0%");
	else
		printf("\nѹ����: %.2lf%c\n", (double)((FileLength - Length) / (double)FileLength) * 100.0, '%');//��Դ�ļ����Ⱥ�ѹ���󳤶�ת��Ϊdouble���ͽ���ѹ���ʼ���
	fclose(fp);
	fclose(compressFile);
	free(Q);
	free(hc);
	free(Map);
}
//�Ѷ������ַ���ת����8��0��1�ַ��������

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

//�������̣����볤��
void MaxMinLength(FILE * File, HTNode * ht, char **hc, short NLeaf, MyType * Max, MyType * Min)
{
	int i;
	MyType length;

	*Max = *Min = strlen(hc[0]);
	for (i = 0; i < NLeaf; i++) {
		length = strlen(hc[i]);
		fwrite(&ht[i].ch, sizeof(MyType), 1, File);    //�ַ��Ͷ�Ӧ��
		fwrite(&length, sizeof(MyType), 1, File);    //���볤��д���ļ�
		if (length > *Max)
			*Max = length;
		if (length < *Min)
			*Min = length;
	}
}
//�ѳ��ֹ����ַ��������ѹ��д���ļ�
short CodeToFile(FILE * fp, char **hc, short n, SeqQueue * Q, MyType * length)
{
	int i;
	char *p;
	MyType j, bits;
	short count = 0;

	for (i = 0; i < n; i++)    // ��n��Ҷ��ѹ����д���ļ�
	{
		for (p = hc[i]; '\0' != *p; p++)
			In_seqQueue(Q, *p);

		while (Q->length > 8) {    // puts("����");
			bits = GetBits(Q);    //����8��Ԫ��
			fputc(bits, fp);    //fwrite(&bits,sizeof(char),1,fp);
			//printf("ѹ�ַ� %c\n",bits);
			count++;
		}
	}    // printf("�������ʣ: %d\n",Q->length);

	*length = Q->length;
	i = 8 - *length;
	bits = GetBits(Q);    //ȡ8������Ӳ���
	for (j = 0; j < i; j++)
		bits = bits << 1;    //printf("ѹ �ַ� %c\n",bits);
	fputc(bits, fp);    //fwrite(&bits,sizeof(char),1,fp);
	count++;    //printf(" ָ �� ��%d \n",ftell(fp));

	InitQueue(Q);
	return count;
}

