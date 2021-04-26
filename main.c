# include "huffman.h"
int main()
{
	char choice;
	while (1) {
		puts(" * * * *霍夫曼压缩* * *");
		puts(" **********************");
		puts(" * *");
		printf(" *  1 压缩  *\n");
		puts(" * *");
		printf(" *  2 解压  *\n");
		puts(" * *");
		printf(" *  3 退出  *\n");
		puts(" * *");
		puts(" **********************");
		printf(" 选择 (1 ~ 3):");
		choice = getchar();
		getchar();
		fflush(stdin);    //清空输入缓冲区域，否则键入的回车符将作为程序结尾处的scanf输入，此函数在stdio.h中
		switch (choice) {
		case '1':
			Compress();
			printf("\n回车继续...\n");
			getchar();
			break;
		case '2':
			UnCompress();
			printf("\n回车继续...\n");
			getchar();
			break;
		case '3':
			return 0;
			break;
		}
	}

	return 0;
}
