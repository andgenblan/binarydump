#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<ctype.h>
#include<unistd.h>

void printBin(uint8_t num) {
	for(int i = 7; i >= 0; i--)
	{
		int bit = (num >> i) & 1;
		printf("%d", bit);
	}
}

int main(int argc, char *argv[]) {
	int ascii = 0;
	int hex = 0;
	int nooff = 0;
	int raw = 0;

	char f;
	while((f = getopt(argc, argv, "axnrh")) != -1)
	{
		switch(f)
		{
		case 'a':
			ascii = 1;
			break;
		case 'x':
			hex = 1;
			break;
		case 'n':
			nooff = 1;
			break;
		case 'r':
			raw = 1;
			break;
		case 'h':
			printf("\nBinary Dump: hexdump for binary\n\n");
			printf("Usage: \n");
			printf("\tbd [options] [file]\n\n");
			printf("Options: \n");
			printf("\t-a\tInclude ASCII\n");
			printf("\t-x\tInclude hexadecimal\n");
			printf("\t-n\tDo not include offset\n");
			printf("\t-r\tRaw binary, no ASCII, hexadecimal, offset, or spaces\n");
			printf("\t-h\tDisplay this help\n\n");
			return 0;
		default:
			printf("%c is not a valid flag\n", f);
			return 1;
		}
	}
	if(raw)
	{
		ascii = 0;
		hex = 0;
		nooff = 1;
	}

	// if(ascii)
	// {
	// 	printf("ascii flag set\n");
	// }
	// if(hex)
	// {
	// 	printf("hex flag set\n");
	// }
	// if(nooff)
	// {
	// 	printf("nooff flag set\n");
	// }
	// printf("optind: %d\n", optind);

	if(argc < 2) {
		printf("Wrong number of arguments\n");
		return 1;
	}

	FILE *file;
	file = fopen(argv[optind], "rb");
	if(file == NULL) {
		printf("Couldn't open file\n");
		return 2;
	}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);

	uint8_t *fileData = (uint8_t *)malloc(size + 1);
	memset(fileData, 0, size+1);
	// printf("%02x\n", fileData[0]);
	
	fread(fileData, 1, size, file);

	for(int x = 0; x < size; x++)
	{
		if(x % 8 == 0)
		{
			if(x % 16 == 0)
			{
				if(x-16 >= 0)
				{
					if(hex)
					{
						printf("  | ");
						for(int y = x-16; y < x; y++)
						{
							if(y % 8 == 0)
							{
								printf("  ");
							}
							printf("%02x ", fileData[y]);
						}
					}
					if(ascii)
					{
						printf("  | ");
						for(int y = x-16; y < x; y++)
						{
							if(y % 8 == 0)
							{
								printf("  ");
							}
							char c = fileData[y];
							if(isprint(c))
							{
								printf("%c ", fileData[y]);
							}
							else
							{
								printf(". ");
							}
						}
					}
				}
				if(!raw)
				{
					printf("\n");
					if(!nooff)
					{
						printf("0x%08x: ", x);
					}
				}
			}
			else
			{
				if(!raw)
				{
					printf("  ");
				}
			}
		}
		printBin(fileData[x]);
		if(!raw)
		{
			printf(" ");
		}

		if(x == size-1)
		{
			// Make the binary even
			if(!raw)
			{
				for(int i = 16 - (x%16) - 1; i > 0; i--)
				{
					printf("         ");
				}
				if((x%16) < 8)
				{
					printf("  ");
				}
			}


			if(hex)
			{
				printf("  | ");
				for(int y = x - (x%16); y <= x; y++)
				{
					if(y % 8 == 0)
					{
						printf("  ");
					}
					printf("%02x ", fileData[y]);
				}
				for(int i = 16 - (x%16) - 1; i > 0; i--)
				{
					printf("   ");
				}
				if((x%16) < 8)
				{
					printf("  ");
				}
			}


			if(ascii)
			{
				printf("  | ");
				for(int y = x - (x % 16); y <= x; y++)
				{
					if(y % 8 == 0)
					{
						printf("  ");
					}
					char c = fileData[y];
					if(isprint(c))
					{
						printf("%c ", fileData[y]);
					}
					else
					{
						printf(". ");
					}
				}
			}
		}
	}
	
	// printBin(fileData[0]);
	
	printf("\n\n");

	free(fileData);
	return 0;
}

