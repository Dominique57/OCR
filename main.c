#include "gtk/gtk.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include "Bmp_Parser.h"
#include "type/image.h"
#include "type/cut.h"
#include "resize.h"

//gcc `pkg-config gtk+-3.0 --cflags` *.c -o `pkg-config gtk+-3.0 --libs`

GtkTextBuffer *NNOutputTest;
GtkTextBuffer *NNOutputTrain;
GtkImage *image_glo;
GtkSpinButton *IterInput;
GtkSwitch *sw;


int toConvert=0;
gchar *filename="icon.svg";

void print_Array(unsigned char *array, size_t w, size_t h)
	//prints all values from an array
{
	size_t k;
	k=0;
	for(size_t i=0; i<h; i++)
	{
			for (size_t j=0; j<w; j++)
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

/*
int test_resize()
{
        Image image;
        unsigned char *filename="dataset/square10.bmp";
        // not very optimized, feel free to optimize
        image.h = getHeight(filename);
        image.w = getWidth(filename);
        unsigned char final_array[ image.h * image.w ];
        //initialization of the pixel array
        image.data = final_array;

        if (parse_bmp(final_array, filename)==1)
        {
                printf("File not found !\n filename=%s\n",filename);
        }
		print_Array(final_array, image.w, image.h);
        Rect rect;
        Cord topleft;
        Cord downright;
		topleft.x=0;
        topleft.y=0;
        downright.x=image.w-1;
        downright.y=image.h-1;
		rect.topLeft=topleft;
		rect.downRight=downright;
        size_t maxlen=image.w;
		if (image.h>image.w)
		{
			maxlen=image.h;
		}
		unsigned char toRet[256];
		resize(image, rect, toRet);
		//print_Array(toRet, 16, 16);
		return 0;
}
*/
char *ReadOutput()
{
	FILE *file=fopen("output.txt", "r");
	if (file==NULL)
		return NULL;
	
	fseek(file, 0, SEEK_END); //go to end
	size_t size=ftell(file); //get size
	rewind(file); //start again

	char *output=malloc(size+1); //+1 for NULL byte

	if(fread(output, 1, size, file)!=size)
		printf("ReadOuput: Error reading file");

	fclose(file);
	output[size]='\0';
	return output;

}

void convert(GtkImage *image_glo, int iterNbr,
		char *text, int useAlgo, int loadSaved)
{
	// Simon's testing
	Image image;
	// not very optimized, feel free to optimize
	image.h = getHeight(filename);
	image.w = getWidth(filename);
	unsigned char final_array[ image.h * image.w ]; 
	//initialization of the pixel array
	image.data = final_array;

	if (parse_bmp(final_array, filename)==1)
	{
		printf("File not found !\n filename=%s\n",filename);
	}
	Image cutted;
	if (useAlgo)
	{
		cutted=cut_noAI(filename);
	}
	else
	{
		cutted = cut_new(filename, text, iterNbr, loadSaved);
	}
	char n[]="converted.bmp";
	array_to_bmp(cutted.data, cutted.w, cutted.h, filename, n);
	gtk_image_set_from_file(GTK_IMAGE(image_glo), n);
}


void load_train_NN(GtkWidget *LoadBtn, GtkImage *image)
{
    filename=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(LoadBtn));
    gtk_image_set_from_file(GTK_IMAGE(image), filename);
	image_glo=image;
}


void train_NN(GtkWidget *Btn, GtkTextBuffer *input_buffer)
{
	//get buffer start and end indexes
	GtkTextIter start;
    GtkTextIter end;
	gtk_text_buffer_get_bounds(input_buffer, &start, &end);

	//now we can get the text
	char *text=gtk_text_buffer_get_text(input_buffer, &start, &end, FALSE);

	//and the for iter number:
	int iterNbr=gtk_spin_button_get_value_as_int(IterInput);
	
	int loadsaved=gtk_switch_get_state(sw);
    convert(image_glo, iterNbr, text, 0, loadsaved);
	
	text=ReadOutput();

	gtk_text_buffer_set_text(NNOutputTrain, text, strlen(text));
	free(text);

}

void test_NN(GtkWidget *Btn)
{
	convert(image_glo, 0, NULL, 0, 1);
	char *text=ReadOutput();
	gtk_text_buffer_set_text(NNOutputTest, text, strlen(text));
	free(text);
}


void test_NN_algo(GtkWidget *Btn)
{
	convert(image_glo, 0, NULL, 1, 0);
	char *text=ReadOutput();
	gtk_text_buffer_set_text(NNOutputTest, text, strlen(text));
	free(text);
}


int main(int argc, char *argv[])
{
    GtkWidget *window;

    gtk_init(&argc, &argv);
    GtkBuilder *builder=gtk_builder_new();

    gtk_builder_add_from_file(builder, "OCRMenu.glade", NULL);
    window=GTK_WIDGET(
    		gtk_builder_get_object(builder, "window"));
	NNOutputTest=GTK_TEXT_BUFFER(
			gtk_builder_get_object(builder, "Network_Output_2"));
	NNOutputTrain=GTK_TEXT_BUFFER(
			gtk_builder_get_object(builder, "Network_Output"));
	IterInput=GTK_SPIN_BUTTON(
			gtk_builder_get_object(builder, "IterInput"));
	sw=GTK_SWITCH(
			gtk_builder_get_object(builder, "loadSaved"));

    gtk_builder_connect_signals(builder, NULL);

    //builder is now useless
    g_object_unref(G_OBJECT(builder));
	//gtk_window_fullscreen(GTK_WINDOW(window));
	gtk_window_maximize(GTK_WINDOW(window));
    gtk_widget_show(window);

    //Running the window
    gtk_main();
    return 0;
}
