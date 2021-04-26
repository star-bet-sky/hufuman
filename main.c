# include "huffman.h"
int main()
{
	char choice;
	while (1) {
		puts(" * * * *»ô·òÂüÑ¹Ëõ* * *");
		puts(" **********************");
		puts(" * *");
		printf(" *  1 Ñ¹Ëõ  *\n");
		puts(" * *");
		printf(" *  2 ½âÑ¹  *\n");
		puts(" * *");
		printf(" *  3 ÍË³ö  *\n");
		puts(" * *");
		puts(" **********************");
		printf(" Ñ¡Ôñ (1 ~ 3):");
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
