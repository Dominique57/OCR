#include "stdio.h"
#include "Bmp_Parser.h"

int main ()
{

	char path[]="~/Test.bmp";


	//not very optimized, feel free to modify if you have an idea
	size_t width=getWidth(path);
	size_t height=getHeight(path);  


	unsigned char final_array[width*height]; //initialization of the pixel array

	if (parse_bmp(final_array, path)==1)
	{
		printf("File not found !");
		return 1;
	}
	return 0;
}
