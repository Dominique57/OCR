#include "gtk/gtk.h"

//gcc `pkg-config gtk+-3.0 --cflags` *.c -o `pkg-config gtk+-3.0 --libs`

int main(int argc, char *argv[]) {

	//Init window itself
	GtkWidget *window;
	
	//Passing args from main to window
 	gtk_init(&argc, &argv);

	//Window properties
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 	gtk_window_set_title(GTK_WINDOW(window), "OCR");
 	gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

	//Creating box cointaining buttons
 	GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,10);
 	gtk_container_add(GTK_CONTAINER(window),box);

	//buffer for path
	GtkTextBuffer* pathBuffer = gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(pathBuffer, "Path :", 6);	

	//Creating buttons and adding them to the box
	GtkWidget* pathBox = gtk_text_view_new_with_buffer(pathBuffer);
	GtkWidget* convertBtn = gtk_button_new_with_label ("Convert");
 	GtkWidget* leaveBtn = gtk_button_new_with_label ("Leave");
 	
	gtk_widget_set_valign(pathBox, GTK_ALIGN_START);
	gtk_widget_set_valign(convertBtn, GTK_ALIGN_START);
 	gtk_widget_set_valign(leaveBtn, GTK_ALIGN_START);
 	
	gtk_container_add(GTK_CONTAINER (box), pathBox);
	gtk_container_add(GTK_CONTAINER (box), convertBtn);
 	gtk_container_add(GTK_CONTAINER (box), leaveBtn);
	
	//Rendering everything
 	gtk_widget_show_all(window);

	//Creating signals
 	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(leaveBtn, "clicked", G_CALLBACK(gtk_main_quit), NULL);

	//Running the window
 	gtk_main();
 	 
 	return 0;
}
