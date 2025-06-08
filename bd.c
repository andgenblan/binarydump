#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<ctype.h>

void printBin(uint8_t num) {
	for(int i = 7; i >= 0; i--)
	{
		int bit = (num >> i) & 1;
		printf("%d", bit);
	}
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Wrong number of arguments\n");
		return 1;
	}

	FILE *file;
	file = fopen(argv[1], "rb");
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
					printf("  | ");
					for(int y = x-16; y < x; y++)
					{
						if(y % 8 == 0)
						{
							printf("  ");
						}
						printf("%02x ", fileData[y]);
					}
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
				printf("\n0x%08x: ", x);
			}
			else
			{
				printf("  ");
			}
		}
		printBin(fileData[x]);
		printf(" ");

		if(x == size-1)
		{
			// Make the binary even
			for(int i = 16 - (x%16) - 1; i > 0; i--)
			{
				printf("         ");
			}
			if((x%16) < 8)
			{
				printf("  ");
			}


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
	
	// printBin(fileData[0]);
	
	printf("\n\n");

	free(fileData);
	return 0;
}

