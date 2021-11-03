#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char* inname;
char* outname;
uint8_t buffer [64];
uint16_t filesize = 0;

void load_file(char* inname, int width)
{
	FILE* file_ptr;
	char c;
	uint16_t i=0;
	int j=0;
	int lines=1;

	printf("Loading file %s.", inname);

	file_ptr = fopen(inname, "rb");

	if (file_ptr == NULL)
	{
	printf("Unable to open file: %s\n", inname);
	printf("Please check the file actually exists\n");
	exit(EXIT_FAILURE);
	// exit will terminate the program here, nothing further will happen
	}
	
	fseek(file_ptr, -width, SEEK_END);
	
	c = fgetc(file_ptr);
	
	while (i < filesize)
	{
		buffer[j] = c;
		i++;
		if (i % width == 0)
		{
			lines++;
			fseek(file_ptr, (-width*lines), SEEK_END);
		}
		c = fgetc(file_ptr);
		j++;
	}
		
	printf("\nFile read successfully!\n");
	fclose(file_ptr);
	printf("\n");
}

void save_file(char* outname, uint8_t* source_data, uint16_t data_size)
{
	FILE* file_ptr;
	file_ptr = fopen(outname, "wb+");
	fseek(file_ptr, 0, SEEK_SET);
	fwrite(source_data, 1, data_size, file_ptr);
	fclose(file_ptr);
}

void main()
{
	int width = 0;
	int height = 0;
	
	inname = malloc(13);
	outname = malloc(13);
	printf("Enter file to convert:\n");
	scanf("%s", inname);
	
	width = 8;
	height = 8;
	
	filesize = width*height;
	printf("Total file size: %u bytes", filesize);
	printf("\n");
	load_file(inname, width);
	
	printf("Enter output name:\n");
	scanf("%s", outname);
	printf("\n");

	save_file(outname, buffer, 64);
	printf("BMP converted to sprite successfully!");
	getch();
}