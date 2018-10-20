#include "gtk/gtk.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

//gcc `pkg-config gtk+-3.0 --cflags` *.c -o `pkg-config gtk+-3.0 --libs`


//void display_image(GtkWidget *image, )

GtkWidget *image;
GtkWidget *pathBtn;



void load()
{
	gchar *filename=gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(pathBtn));
	gtk_image_set_from_file(GTK_IMAGE(image), filename);
	g_free(filename);
	//gtk_widget_queue_draw(image);
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
	image=gtk_image_new_from_file("icon.svg");
	

	//Creating buttons and adding them to the box
	pathBtn = gtk_file_chooser_button_new (("Load image"),GTK_FILE_CHOOSER_ACTION_OPEN);


	GtkWidget* convertBtn = gtk_button_new_with_label ("Convert");
 	GtkWidget* leaveBtn = gtk_button_new_with_label ("Leave");
 	
	gtk_widget_set_valign(pathBtn, GTK_ALIGN_START);
	gtk_widget_set_valign(convertBtn, GTK_ALIGN_START);
 	gtk_widget_set_valign(leaveBtn, GTK_ALIGN_START);
 	
	gtk_container_add(GTK_CONTAINER (box), pathBtn);
	gtk_container_add(GTK_CONTAINER (box), convertBtn);
 	gtk_container_add(GTK_CONTAINER (box), leaveBtn);
	gtk_container_add(GTK_CONTAINER (ibox), image);
	gtk_container_add(GTK_CONTAINER (box), ibox);
	

	//Rendering everything
 	gtk_widget_show_all(window);

	//Creating signals
 	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(leaveBtn, "clicked", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(pathBtn, "file-set", G_CALLBACK(load), NULL);

	//Running the window
 	gtk_main();
 	 
 	return 0;
}
