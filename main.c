# include "huffman.h"
int main()
{
	char choice;
	while (1) {
		puts(" * * * *������ѹ��* * *");
		puts(" **********************");
		puts(" * *");
		printf(" *  1 ѹ��  *\n");
		puts(" * *");
		printf(" *  2 ��ѹ  *\n");
		puts(" * *");
		printf(" *  3 �˳�  *\n");
		puts(" * *");
		puts(" **********************");
		printf(" ѡ�� (1 ~ 3):");
		choice = getchar();
		getchar();
		fflush(stdin);    
		switch (choice) {
		case '1':
			Compress();
			break;
		case '2':
			UnCompress();
			break;
		case '3':
			return 0;
			break;
		}
	}

	return 0;
}
