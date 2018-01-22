#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

struct timeval start;
struct timeval stop;

struct {
	float x;
	float alt;
	float land;
	float scal_x;
	float scal_y;
} glob;

double DegreesToRadians( double degrees )
{
    return((double)((double)degrees * ( (double)3.1415/(double)180.0 )));
}

void Rotate( cairo_surface_t *image, int degrees, cairo_t *cr)
{
    int w, h;

    cr = cairo_create(image);
    w = cairo_image_surface_get_width (image);
    h = cairo_image_surface_get_height (image);
    cairo_set_source_surface (cr, image, 1200, 0);

    //cairo_translate(cr, w/2.0, h/2.0);
    //cairo_rotate(cr, DegreesToRadians( degrees ));
    //cairo_translate(cr, - w/2.0, -h/2.0);

    cairo_paint (cr);
}

void do_drawing(cairo_t *cr, int w,
	int h, cairo_operator_t op)
{
	
	cairo_save(cr);
	cairo_surface_t *image2;
	image2 = cairo_image_surface_create_from_png ("images/sky.png");
	cairo_set_source_surface (cr, image2, 0,0);
	cairo_rectangle (cr, 0,0,1200,300);
	cairo_clip (cr);
	cairo_new_path (cr); 
	cairo_paint (cr);
	cairo_restore(cr);

	cairo_save(cr);
	cairo_surface_t *image3;
	image3 = cairo_image_surface_create_from_png ("images/forrest2.png");
	cairo_rectangle (cr, 0,0,1200,300);
	cairo_clip (cr);
	cairo_new_path (cr);
	cairo_scale (cr, 0.6, 0.2);
	cairo_set_source_surface (cr, image3, -100, 1100);
	cairo_paint (cr);
	cairo_restore(cr);

	cairo_save(cr);
	cairo_surface_t *image4;
	image4 = cairo_image_surface_create_from_png ("images/roda_outside.png");
	cairo_rectangle (cr, 1200, 0 , 400,400);
	cairo_clip (cr);
	cairo_new_path (cr);
	//cairo_scale (cr, 0, 0.65);
	cairo_set_source_surface (cr, image4, 1200/1, 0);
	cairo_paint (cr);
	cairo_restore(cr);


	cairo_save(cr);
	cairo_surface_t *image5;
	image5 = cairo_image_surface_create_from_png ("images/roda_heading.png");
	
	int ww, hh, degrees = 5;
   
    ww = cairo_image_surface_get_width (image5);
    hh = cairo_image_surface_get_height (image5);

    cairo_translate(cr, w/2.0, h/2.0);
    cairo_rotate(cr, DegreesToRadians( degrees ));
    cairo_translate(cr, - w/2.0, -h/2.0);

    cairo_set_source_surface(cr, image5,  0, 0);
    cairo_paint (cr);


	cairo_rectangle (cr, 1200, 0 , 400,400);
	cairo_clip (cr);
	cairo_new_path (cr);
	
	
	
	cairo_restore(cr);



	

    
	


	cairo_save(cr);
	cairo_set_line_width(cr, 1); 
	cairo_move_to (cr, 0, 185);
	cairo_line_to (cr, 1000, 300-30);
	cairo_stroke (cr);
	cairo_rectangle (cr, 1000, 300-30, 200, 10 );
	cairo_set_source_rgba (cr,0.756, 0.682, 0.682,  1);
	cairo_fill (cr);
	cairo_restore(cr);
	
	cairo_save(cr);
	cairo_surface_t *image;
	cairo_rectangle (cr, 0,0,1200,300);
	cairo_clip (cr);
	cairo_new_path (cr); 
	image = cairo_image_surface_create_from_png ("images/plane.png");
	cairo_scale (cr, glob.scal_x, glob.scal_y);
	printf("pos %f %f\n",glob.x, glob.alt);
	cairo_set_source_surface (cr, image, glob.x, glob.alt);
	cairo_paint (cr);
	cairo_restore(cr);

	cairo_surface_destroy (image);
	cairo_surface_destroy (image2);
	cairo_surface_destroy (image3);
	cairo_surface_destroy (image4);
	cairo_surface_destroy (image5);

}

float timedifference_msec(struct timeval t0, struct timeval t1)
{
	return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
	gpointer user_data)
{      

	gettimeofday(&start, NULL);

	GtkWidget *win = gtk_widget_get_toplevel(widget);

	int width, height;
	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
	
	if (glob.land > glob.alt)
	{
		glob.x += 1000*glob.scal_x;
		glob.alt += 52.4*glob.scal_x;
	}else if (glob.x < 1250/glob.scal_x){
		glob.x += 1000*glob.scal_x;
	}

	do_drawing(cr, width, height, CAIRO_OPERATOR_DEST_OVER );
	

	gettimeofday(&stop, NULL);
	float elapsed = timedifference_msec(start, stop);
	printf("took %f\n", elapsed);
	
	return FALSE;
}

static gboolean time_handler(GtkWidget *widget)
{

	gtk_widget_queue_draw(widget);

	return TRUE;
}

int main(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *darea;  

	glob.scal_x = 0.05;
	glob.scal_y = 0.08;
	glob.x = 0;
	glob.alt = (300-52.4-80)/glob.scal_y;
	glob.land = (300-52.4-0)/glob.scal_y;
	

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER (window), darea);

	g_signal_connect(G_OBJECT(darea), "draw", 
		G_CALLBACK(on_draw_event), NULL);  
	g_signal_connect(window, "destroy",
		G_CALLBACK(gtk_main_quit), NULL);

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 1600, 600);
	gtk_window_set_title(GTK_WINDOW(window), "ILS");

	g_timeout_add(5, (GSourceFunc) time_handler, (gpointer) window);
	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
