#include "gtk/gtk.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>
#include "Bmp_Parser.h"
#include "type/image.h"
#include "RLSA/RLSA.h"
#include "cut.h"
#include "resize.h"

//gcc `pkg-config gtk+-3.0 --cflags` *.c -o `pkg-config gtk+-3.0 --libs`



GtkWidget *image_glo;
GtkWidget *pathBtn;

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

Image tryRLSA(char *path)
{
	Image image;
	image.w = getWidth(path);
	image.h = getHeight(path);
	unsigned char data[image.w * image.h];
	if (parse_bmp(data, path)==1)
	{
		printf("File not found !\n filename=%s\n",path);
	}
	image.data = data;
	print_Array(image.data, image.w, image.h);
	printf("\n\n\n");
	Image result = RLSA_launch(image, 200, 200);
	return result;
}


void load()
{
	filename=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(pathBtn));
	gtk_image_set_from_file(GTK_IMAGE(image_glo), filename);
	//g_free(filename);
}

void convert()
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
	Image cutted = cut(filename);
	char n[]="converted.bmp";
	array_to_bmp(cutted.data, cutted.w, cutted.h, filename, n);
	gtk_image_set_from_file(GTK_IMAGE(image_glo), n);
	
}

int main(int argc, char *argv[]) {


	//Init window itself
	GtkWidget *window;
	GError **error=NULL;
	
	//Passing args from main to window
 	gtk_init(&argc, &argv);

	//Window properties
	GdkPixbuf *icon=gdk_pixbuf_new_from_file("icon.svg", error);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 	gtk_window_set_title(GTK_WINDOW(window), "OCR");
 	gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
	gtk_window_set_icon(GTK_WINDOW(window), icon);

	//Creating box cointaining buttons and image
 	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);
	GtkWidget* ibox = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
 	gtk_container_add(GTK_CONTAINER(window),box);
	

	//init image choice
	image_glo=gtk_image_new_from_file("icon.svg");
	
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

	//Creating buttons and adding them to the box
	pathBtn = gtk_file_chooser_button_new (("Load image"),action);


	GtkWidget* convertBtn = gtk_button_new_with_label ("Convert");
 	GtkWidget* leaveBtn = gtk_button_new_with_label ("Leave");
 	
	gtk_widget_set_valign(pathBtn, GTK_ALIGN_START);
	gtk_widget_set_valign(convertBtn, GTK_ALIGN_START);
 	gtk_widget_set_valign(leaveBtn, GTK_ALIGN_START);
 	
	gtk_container_add(GTK_CONTAINER (box), pathBtn);
	gtk_container_add(GTK_CONTAINER (box), convertBtn);
 	gtk_container_add(GTK_CONTAINER (box), leaveBtn);
	gtk_container_add(GTK_CONTAINER (ibox), image_glo);
	gtk_container_add(GTK_CONTAINER (box), ibox);
	

	//Rendering everything
 	gtk_widget_show_all(window);

	//Creating signals
 	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(leaveBtn, "clicked", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(pathBtn, "file-set", G_CALLBACK(load), NULL);
	g_signal_connect(convertBtn, "clicked", G_CALLBACK(convert), NULL);

	//Running the window
 	gtk_main();
 	 
 	return 0;
}
