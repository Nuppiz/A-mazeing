#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char inputname[13];
char outputname[13];
char inname[13];
char outname[13];
uint8_t* buffer;
uint16_t filesize = 0;
int width;
int height;
int checksum1;
int checksum2;

void load_file(char* inname)
{
	FILE* file_ptr;
	char c;
	uint16_t i=0;
	int j=0;
	int lines=1;

	printf("Loading file %s...\n", inname);

	file_ptr = fopen(inname, "rb");
	
	//check that the file actually exists
	if (file_ptr == NULL)
	{
		printf("Unable to open file: %s\n", inname);
		printf("Please check the file actually exists\n");
		exit(EXIT_FAILURE);
		// exit will terminate the program here, nothing further will happen
	}
	
	fseek(file_ptr, 0, SEEK_SET);
	fread(&checksum1, 2, 1, file_ptr); //check for BMP file header
	fseek(file_ptr, 28, SEEK_SET);
	fread(&checksum2, 1, 1, file_ptr); // check bit depth
	
	//if file header or bit depth is wrong, then exit
	if (checksum1 != 19778 || checksum2 != 8)
	{
		printf("Fatal error: %s is not a valid 8-bit bitmap!\n", inname);
		printf("This program will now exit.\n");
		exit(EXIT_FAILURE);
		// exit will terminate the program here, nothing further will happen
	}
	
	fseek(file_ptr, 18, SEEK_SET);
	fread(&width, 2, 1, file_ptr);
	printf("Checking file width... %upx\n", width);
	fseek(file_ptr, 22, SEEK_SET);
	fread(&height, 2, 1, file_ptr);
	printf("Checking file height... %upx\n", height);
	printf("Checking file dimensions complete!\n");
	
	filesize = width * height;
	printf("Total file size: %u bytes\n", filesize);
	printf("\n");
	
	buffer = malloc(filesize);
	printf("Allocating memory...");
	printf("\n");
	
	printf("Starting to read first line...\n");
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
	printf("Enter file to convert (without file extension):\n");
	scanf("%s", inputname);
	sprintf(inname, "%s.bmp", inputname);
	
	load_file(inname);
	
	printf("Enter output name (without file extension)\n");
	printf("or write 'same' to save with the BMP name:\n");
	scanf("%s", outputname);
	
	if (strcmp ("same", outputname) == 0)
		sprintf(outname, "%s.7UP", inputname);
	
	else
		sprintf(outname, "%s.7UP", outputname);
	
	printf("Writing file: %s\n", outname);
	save_file(outname, buffer, filesize);
	printf("BMP successfully converted into %s!\n", outname);
	getchar();
}