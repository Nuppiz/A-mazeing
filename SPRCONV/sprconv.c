#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char inputname[13];
char outputname[13];
char inname[13];
char outname[13];
uint8_t* far buffer;
uint16_t filesize = 0;
int width;
int height;
int checksum1;
int checksum2;
int running = 1;

void load_file(char* inname)
{
	FILE* file_ptr;

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
	printf("Checking file width... %dpx\n", width);
	fseek(file_ptr, 22, SEEK_SET);
	fread(&height, 2, 1, file_ptr);
	printf("Checking file height... %dpx\n", height);
	printf("Checking file dimensions complete!\n");
	
	filesize = width * height;
	printf("Total file size: %u bytes\n", filesize);
	printf("\n");
	
    	printf("Allocating memory...");
	buffer = (uint8_t far*)malloc(filesize);
	printf("\n");
	
	printf("Starting to read first line...\n");
	
	fseek(file_ptr, 1078, SEEK_SET);
	fread(buffer, 1, filesize, file_ptr);
	fclose(file_ptr);
	
	printf("\n%u bytes read!\n", filesize);
	printf("\nFile read successfully!\n");
	fclose(file_ptr);
	printf("\n");
}

void save_file(char* outname)
{
	int16_t y, x;
	FILE* file_ptr = fopen(outname, "wb+");
	
	for (y = height-1; y >= 0; y--)
	{
		for (x = 0; x < width; x++)
		{
			fputc(buffer[(y*width)+x], file_ptr);
		}
	}
	fclose(file_ptr);
}

void menu()
{
	char response;

	// take in the file input name
	printf("Enter file to convert (without file extension):\n");
	scanf("%s", inputname);
	sprintf(inname, "%s.bmp", inputname);
	
	// load file into buffer
	load_file(inname);
    
	// take in the file output name
	printf("Enter output name (without file extension)\n");
	printf("or write 'same' to save with the BMP name:\n");
	scanf("%s", outputname);
	
	if (strcmp ("same", outputname) == 0)
		sprintf(outname, "%s.7UP", inputname);
	
	else
		sprintf(outname, "%s.7UP", outputname);
	
	printf("Writing file: %s\n", outname);

	// save file to 7UP format
	save_file(outname);

	printf("BMP successfully converted into %s!\n", outname);

	//clear memory
	inputname [0] = '\0';
	outputname [0] = '\0';
	inname [0] = '\0';
	outname [0] = '\0';
	farfree(buffer);
    
	// ask if user wants to convert another file, or quit
	printf("Y to convert another file, or N to quit.\n");
	scanf (" %c", &response);
	if (response == 'y' || response == 'Y')
		running = 1;
	else if (response == 'n' || response == 'N')
		running = 0;
}

int main()
{
	while (running == 1)
		menu();
	return 0;
}
