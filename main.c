#include "stdio.h"
#include "Bmp_Parser.h"



void print_Array(unsigned char array[], size_t w, size_t h) //prints all values from an array
{
	size_t k;
	k=0;
	for(size_t i=0; i<w; i++)
	{
			for (size_t j=0; j<h; j++)
			{
				printf("%d|",array[k]);
				k++;
			}
			printf("\n");
	}
}


int main ()
{

	char path[]="TEST3.bmp";


	//not very optimized, feel free to modify if you have an idea
	size_t width=getWidth(path);
	size_t height=getHeight(path);  


	unsigned char final_array[width*height]; //initialization of the pixel array

	if (parse_bmp(final_array, path)==1)
	{
		printf("File not found !\n");
		return 1;
	}
	//print_Array(final_array, width, height); //debug
	return 0;
}
