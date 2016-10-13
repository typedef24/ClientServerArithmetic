#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <errno.h>
#include <limits.h>

GtkWidget *result_entry, *x_entry, *y_entry;
	typedef struct{ 
		float x;
		float y;
		int operator;
		float result;
	}arithmetic_operatn;
arithmetic_operatn operatn;
int checker = 0;

static void destroy(GtkWidget *widget, gpointer data);
static void clearer(GtkWidget* button, gpointer data);
static void plus_toggled(GtkWidget* button, gpointer data);
static void mult_toggled(GtkWidget* button, gpointer data);
static void subtract_toggled(GtkWidget* button, gpointer data);
static void divide_toggled(GtkWidget* button, gpointer data);
static void modulus_toggled(GtkWidget* button, gpointer data);
static void sender(GtkWidget* button, gpointer data);
static void about_me(GtkWidget* button, gpointer data);

int main(int argc, char* argv[]){
	GtkWidget *window, *vbox, *plus_radio, *subtract_radio, *mult_radio;
	GtkWidget *divide_radio, *modulus_radio, *radio, *hbox1, *hbox2, *hbox3;
	GtkWidget *x_label, *y_label, *about, *about_img, *about_label;
	GtkWidget *send, *clear, *send_img, *send_label, *clear_img, *clear_label;
	GtkWidget *send_hbox, *clear_hbox, *about_hbox;
	//initialize gtk+ library
	gtk_init(&argc, &argv);
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Arithmetic Client");
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	g_signal_connect(G_OBJECT(window),"destroy", G_CALLBACK(destroy), NULL);
	gtk_window_set_gravity (GTK_WINDOW(window), GDK_GRAVITY_CENTER);
	
	//create result entry
	result_entry = gtk_entry_new();
	//create the boxes
	vbox = gtk_vbox_new (FALSE, 5);
	hbox1 = gtk_hbox_new (FALSE, 0);
	hbox2 = gtk_hbox_new (FALSE, 5);
	hbox3 = gtk_hbox_new (FALSE, 5);
	//fill the vbox with the entry & hboxes
	gtk_box_pack_start(GTK_BOX(vbox), result_entry, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 3);
	
	/* Create 6 radio buttons where the others join the reference radio button,
 	   radio's group. */
	radio = gtk_radio_button_new (NULL);
	plus_radio = gtk_radio_button_new_with_label_from_widget (
				GTK_RADIO_BUTTON (radio), "+");
	subtract_radio = gtk_radio_button_new_with_label_from_widget (	
				GTK_RADIO_BUTTON (radio),"-");
	mult_radio = gtk_radio_button_new_with_label_from_widget (
				GTK_RADIO_BUTTON (radio),"X");
	divide_radio = gtk_radio_button_new_with_label_from_widget (
				GTK_RADIO_BUTTON (radio),"/");
	modulus_radio = gtk_radio_button_new_with_label_from_widget (
				GTK_RADIO_BUTTON (radio),"%");
	//create the label x= & y= and their entries. pack them 2 hbox1 
	x_label = gtk_label_new("X=");
	y_label = gtk_label_new("Y=");
	x_entry = gtk_entry_new();
	gtk_widget_grab_focus(x_entry);
	y_entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox1), x_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox1), x_entry, TRUE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(hbox1), y_label, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(hbox1), y_entry, TRUE, FALSE, 0);

	//fill hbox2 with the radio buttons
	gtk_box_pack_start(GTK_BOX(hbox2), plus_radio, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox2), subtract_radio, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox2), mult_radio, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox2), divide_radio, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox2), modulus_radio, FALSE, FALSE, 5);
	g_signal_connect (G_OBJECT (plus_radio), "toggled",G_CALLBACK (plus_toggled),
			  NULL);
	g_signal_connect (G_OBJECT (subtract_radio), "toggled",
			  G_CALLBACK (subtract_toggled), NULL);
	g_signal_connect (G_OBJECT (mult_radio), "toggled",G_CALLBACK (mult_toggled),
			  NULL);
	g_signal_connect (G_OBJECT (divide_radio), "toggled",
			  G_CALLBACK (divide_toggled), NULL);
	g_signal_connect (G_OBJECT (modulus_radio), "toggled",
			  G_CALLBACK (modulus_toggled), NULL);
	//create send & clear buttons & pack them into hbox3
	send = gtk_button_new();
	send_label = gtk_label_new("Send");
	send_img = gtk_image_new_from_stock(GTK_STOCK_NETWORK, 2);
	send_hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(send_hbox), send_img, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(send_hbox), send_label, FALSE, FALSE, 3);
	gtk_container_add(GTK_CONTAINER(send), send_hbox);
	clear = gtk_button_new();
	clear_label = gtk_label_new("Clear");
	clear_img = gtk_image_new_from_stock(GTK_STOCK_CLEAR, 2);
	clear_hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(clear_hbox), clear_img, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(clear_hbox), clear_label, FALSE, FALSE, 3);
	gtk_container_add(GTK_CONTAINER(clear), clear_hbox);
	about = gtk_button_new();
	about_label = gtk_label_new("About");
	about_img = gtk_image_new_from_stock(GTK_STOCK_ABOUT, 2);
	about_hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(about_hbox), about_img, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(about_hbox), about_label, FALSE, FALSE, 3);
	gtk_container_add(GTK_CONTAINER(about), about_hbox);

	gtk_box_pack_start(GTK_BOX(hbox3), send, TRUE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(hbox3), about, TRUE, TRUE, 2);
	gtk_box_pack_start(GTK_BOX(hbox3), clear, TRUE, TRUE, 2);
	g_signal_connect (G_OBJECT (clear), "clicked", G_CALLBACK (clearer), 
                          NULL);
	g_signal_connect (G_OBJECT (send), "clicked", G_CALLBACK (sender), 
                          NULL);
	g_signal_connect (G_OBJECT (about), "clicked", G_CALLBACK (about_me), 
                          NULL);
	gtk_container_add (GTK_CONTAINER (window), vbox);
	gtk_widget_show_all (window);
	gtk_main ();
return 0;
}

//clear(): clears all entries
static void clearer(GtkWidget* button, gpointer data){
	gtk_entry_set_text(GTK_ENTRY(result_entry), " ");
	gtk_entry_set_text(GTK_ENTRY(x_entry), " ");
	gtk_entry_set_text(GTK_ENTRY(y_entry), " ");
}

static void plus_toggled(GtkWidget* button, gpointer data){
	operatn.operator = 1;
	checker++;
}
static void subtract_toggled(GtkWidget* button, gpointer data){
	operatn.operator = 2;
	checker++;
}
static void mult_toggled(GtkWidget* button, gpointer data){
	operatn.operator = 3;
	checker++;
}
static void divide_toggled(GtkWidget* button, gpointer data){
	operatn.operator = 4;
	checker++;
}
static void modulus_toggled(GtkWidget* button, gpointer data){
	operatn.operator = 5;
	checker++;
}

//sender(): connects & communicate with server
static void sender(GtkWidget* button, gpointer data){
	//declaring socket varables
	int sockfd;	//socket file descriptor
	int server_len;	//length of server's address
	int result;
	struct sockaddr_un server_addr;		//server's address

	char *endptr, output_str[10], *result_str;
	gtk_entry_set_text(GTK_ENTRY(result_entry), " ");	//clear result entry
	/* if operator is not selected, report error and exit */
	if(checker == 0){
		gtk_entry_set_text(GTK_ENTRY(result_entry), 
				   "Arithmetic operator not selected!");
		return;	
	}
	errno = 0;
	operatn.x = strtod(gtk_entry_get_text(GTK_ENTRY(x_entry)), &endptr);
 	 /* Check for various possible errors */
           if ((errno == ERANGE && (operatn.x == DBL_MAX || operatn.x == DBL_MIN))
                   || (errno != 0 && operatn.x == 0)) {
               	gtk_entry_set_text(GTK_ENTRY(result_entry), "Invalid inputs!");
		errno = 0;
		return;	
           }
           if (endptr == gtk_entry_get_text(GTK_ENTRY(x_entry))) {
               gtk_entry_set_text(GTK_ENTRY(result_entry), "No digits entered!");
		errno = 0;
		return;	
           }	
	operatn.y = strtod(gtk_entry_get_text(GTK_ENTRY(y_entry)), &endptr);
 	 /* Check for various possible errors */
            if ((errno == ERANGE && (operatn.y == DBL_MAX || operatn.y == DBL_MIN))
                   || (errno != 0 && operatn.y == 0)) {
               	gtk_entry_set_text(GTK_ENTRY(result_entry), "Invalid inputs!");
		errno = 0;
		return;	
           }
           if (endptr == gtk_entry_get_text(GTK_ENTRY(y_entry))) {
               gtk_entry_set_text(GTK_ENTRY(result_entry), "No digits entered!");
		errno = 0;
		return;	
           }
	//check divisibility by 0
	if((operatn.operator == 4 || operatn.operator == 5) && operatn.y == 0){
               	gtk_entry_set_text(GTK_ENTRY(result_entry), 
				   "Forbiden operation: division by 0");
		return;	
	}

	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);	//create client socket
	//initializing server's address structure
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "arithmeticServer_socket");
	server_len = sizeof(server_addr);
	
	//connect to server
	if((result = connect(sockfd, (struct sockaddr *)&server_addr, server_len))<0){
		printf("\a");
               	gtk_entry_set_text(GTK_ENTRY(result_entry), 
				   "Connection to server failed!");
		return;	
	}
	//write & read data from socket
  	write(sockfd, &operatn, sizeof(arithmetic_operatn));	
	read(sockfd, &operatn.result, sizeof(float));
	strcpy(output_str, "Server>> ");
	if((result_str = (char *) malloc(sizeof(float) + 5)) == NULL){
		printf("\a");
               	gtk_entry_set_text(GTK_ENTRY(result_entry), 
				   "An error occured in the process!");
		return;			
	}
	sprintf(result_str, "%f", operatn.result);
	strcat(output_str, result_str);
	gtk_entry_set_text(GTK_ENTRY(result_entry), output_str);
	close(sockfd);
}

//destroy(): closes the program window
static void destroy(GtkWidget *widget, gpointer data){
	gtk_main_quit();
}

//about_me(): gives info about programmer
static void about_me(GtkWidget* button, gpointer data){
	GtkWidget *dialog;
	GtkWidget *logo;
	GError *error = NULL;
	const gchar *authors[] = {"-> MOFORKENG STEPHEN NEMBO",
				  "    A.K.A: typedef Stephen Stivo;", NULL};
	const gchar *documenters[] = {"-> MOFORKENG STEPHEN NEMBO",
				      "    A.K.A: typedef Stephen Stivo;", NULL};
	dialog = gtk_about_dialog_new ();
	//logo = gtk_image_new_from_stock (GTK_STOCK_NETWORK, 5);
	//gtk_about_dialog_set_logo (GTK_ABOUT_DIALOG (dialog), GDK_PIXBUF(logo));

	/*logo = gdk_pixbuf_new_from_file ("typedef.jpg", &error);
	gdk_pixbuf_scale_simple(logo, 1, 1, GDK_INTERP_BILINEAR);*/
	
	/* Set the application logo or handle the error. */
	/*if (error == NULL)
		gtk_about_dialog_set_logo (GTK_ABOUT_DIALOG (dialog), logo);
	else{
		if (error->domain == GDK_PIXBUF_ERROR)
			g_print ("GdkPixbufError: %s\n", error->message);
		else if (error->domain == G_FILE_ERROR)
			g_print ("GFileError: %s\n", error->message);
		else
			g_print ("An error in the domain: %d has occurred!\n",
				  error->domain);
		g_error_free (error);
	}*/
	/* Set application data that will be displayed in the main dialog. */
	gtk_about_dialog_set_name (GTK_ABOUT_DIALOG (dialog),
				   "Arithmetic client");
	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (dialog), "1.0");
	gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (dialog),
					"(C) 2016 Arithmetic client");
	gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (dialog),
	 	"Seeing the whole World from one position!");
	/* Set the license text, which is usually loaded from a file. Also, set the
	* web site address and label. */
	gtk_about_dialog_set_license (GTK_ABOUT_DIALOG (dialog), "This is a free open source product");
	/* Set the application authors, documenters and translators. */
	gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (dialog), authors);
	gtk_about_dialog_set_documenters (GTK_ABOUT_DIALOG (dialog), documenters);
	gtk_about_dialog_set_translator_credits (GTK_ABOUT_DIALOG (dialog),
						 "Arithmetic client is currently just in one language, the English language.");
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
}
