#include <stdio.h>
#include "Bmp_Parser.h"
#include "type/image.h"
#include "RLSA/RLSA.h"

void print_Array(unsigned char *array, size_t w, size_t h) //prints all values from an array
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

void print_image(Image im) //prints all values from an array
{
    unsigned char *array = im.data;
    size_t h = im.h;
    size_t w = im.w;
    size_t k;
    k=0;
    for(size_t i=0; i<h; i++)
    {
        for (size_t j=0; j<237; j++)
        {
            printf("%d",array[k]);
            k++;
        }
        for (size_t l = 237; l <w ; ++l)
        {
            k++;
        }
        printf("\n");
    }
}

Image tryRLSA()
{
	char path[] = "dataset/testTEXT2.bmp";
	Image image;
	image.w = getWidth(path);
	image.h = getHeight(path);
	unsigned char data[image.w * image.h];
	if (parse_bmp(data, path)==1)
	{
		printf("File not found !\n");
	}
	image.data = data;
	print_Array(image.data, image.w, image.h);
	printf("\n\n\n");
	Image result = RLSA_launch(image, 200, 200);
	return result;
}


int main ()
{
	// Simon's testing
	char path[]="dataset/TEST3.bmp";

	Image image;
	// not very optimized, feel free to optimize
	image.h = getHeight(path);
	image.w = getWidth(path);
	unsigned char final_array[ image.h * image.w ]; //initialization of the pixel array
	image.data = final_array;

	if (parse_bmp(final_array, path)==1)
	{
		printf("File not found !\n");
		return 1;
	}
	//print_Array(image.data, image.w, image.h); //debug



	Image test = tryRLSA();
	print_Array(test.data, test.w, test.h);
	array_to_bmp(test.data, sizeof(test.data), sizeof(test.data[0]), path); //call with array dimensions
	return 0;
}
