#include "ocr.h"
#include "detection_grid/edge_detection/sobel_operator.h"
#include "detection_grid/hough/hough.h"
#include "detection_grid/segmentation/segmentation.h"
#include "image_process/rotation_scale/rotation.h"
#include "image_process/rotation_scale/scale.h"
#include "image_process/color_treatement/otsu.h"
#include "image_process/color_treatement/grayscale.h"
#include "image_process/color_treatement/blur.h"
#include "image_process/color_treatement/correction.h"
#include "solver/solver.h"
#include "solver/load_save.h"
#include "tools.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <gtk/gtk.h>

#include "err.h"
#include "math.h"

#include <sys/types.h>
#include <sys/stat.h>

GtkBuilder *builder;
GtkWindow *mainWindow;
GtkStack* stack;
GtkWidget* selectWindow;
GtkWidget* solveWindow;
GtkStack* solveStack;

GtkFileChooser *fileChooser;
GtkButton *buttonSelect;
GtkButton *buttonSolve;
GtkButton *buttonClose;
GtkButton *buttonSolving;
GtkButton *buttonRotate;

GtkSpinner* spinner;

GFile *pathCur;
GFile *path;

GdkPixbuf *resolved;

GtkImage *imageUnresolved;
GtkImage *imageResolved;

void on_stack_solve()
{
  gtk_stack_set_visible_child(stack, solveWindow);
}

void on_stack_select()
{
  gtk_stack_set_visible_child(stack, selectWindow);
}

void on_solve()
{
  if (path != NULL)
    {
      gtk_stack_set_visible_child(solveStack, GTK_WIDGET(spinner));

      gtk_spinner_start(spinner);
      
      char *textpath = g_file_get_relative_path(pathCur, path);
      launch(textpath);
      g_free(textpath);

      resolved = gdk_pixbuf_new_from_file_at_scale("out/resolve.bmp", 500, 500, TRUE, NULL);

      gtk_image_set_from_pixbuf(imageResolved, resolved);

      gtk_spinner_stop(spinner);

      gtk_stack_set_visible_child(solveStack, GTK_WIDGET(imageResolved));
    }
}

void on_rotate()
{
  if (resolved)
    {
      resolved = gdk_pixbuf_rotate_simple(resolved, GDK_PIXBUF_ROTATE_CLOCKWISE);

      gtk_image_set_from_pixbuf(imageResolved, resolved);
    }
}

void on_choice(GtkFileChooser *fileChooser)
{
  path = gtk_file_chooser_get_file(fileChooser);

  char *textpath = g_file_get_path(path);

  GdkPixbuf* unresolved = gdk_pixbuf_new_from_file_at_scale(textpath, 500, 500, TRUE, NULL);

  gtk_image_set_from_pixbuf(imageUnresolved, unresolved);
  
  g_free(textpath);
}

void event_loop(SDL_Renderer* renderer, SDL_Texture* texture)
{
   SDL_Event event;
   while(1)
   {
       SDL_WaitEvent(&event);
       switch(event.type)
       {
            case SDL_QUIT:
               return;
            case SDL_WINDOWEVENT:
               if(event.window.event == SDL_WINDOWEVENT_RESIZED)
               {
                   draw(renderer, texture);
               }
               break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    return;
                break;
       }
   }
}

int usage()
{
    printf("%s\n", "Name : OCR");
    printf("%s\n", "Description : Sudoku Solver");
    printf("%s\n", "Usage : [mode] [image] [options]");
    printf("%s\n", "        [mode] : color -> color processing");
    printf("%s\n", "                 rotate -> manual rotation of an image (the degree of the rotation is given in option)");
    printf("%s\n", "        [options] : ONLY FOR THE ROTATION : the degree of the rotation");
    return EXIT_FAILURE;
}

int rotate(char* path, double degree)
{
    SDL_Surface* surface = load_image(path);

    SDL_Surface* surface_rotated = rotation(surface, degree);

    SDL_SaveBMP(surface_rotated, "out.bmp");
    SDL_FreeSurface(surface_rotated);
    return EXIT_SUCCESS;
}

int color_treatement(char* path)
{
    mkdir("out", S_IRWXU);
    SDL_Surface* surface = load_image(path);
    if(surface->w > 1500)
        surface = resize(surface, surface->w/2, surface->h/2);

    SDL_Surface* sgamma = c_gamma(surface);
    SDL_SaveBMP(sgamma, "out/gamma.bmp");
    SDL_Surface* scontrast = c_contrast(sgamma);
    SDL_SaveBMP(scontrast, "out/contrast.bmp");
    grayscale(scontrast);
    SDL_SaveBMP(scontrast, "out/greyscale.bmp");
    SDL_Surface* sblur = blur(scontrast);
    SDL_SaveBMP(sblur, "out/blur.bmp");
    otsu(sblur);
    SDL_SaveBMP(sblur, "out/out.bmp");
    
    SDL_FreeSurface(surface);
    SDL_FreeSurface(sgamma);
    SDL_FreeSurface(scontrast);
    SDL_FreeSurface(sblur);

    return EXIT_SUCCESS;
}

int detection()
{
    SDL_Surface* surface = load_image("out/out.bmp");
    SDL_Surface* sobel = sobel_operator(surface);
    hough(sobel);
    splitting("out/grid.bmp");
    SDL_FreeSurface(surface);
    SDL_FreeSurface(sobel);
    return EXIT_SUCCESS;
}

int solve()
{
    int gridsolved[9][9];
    int gridunsolved[9][9];
    load_grid("given_grid/grid_01", gridunsolved);
    load_grid("given_grid/grid_01", gridsolved);
    Solve(gridsolved,0);
    save_grid(gridsolved, "out/grid.result");
    render_grid(gridunsolved,gridsolved);
    return EXIT_SUCCESS;
}

int launch(char* path)
{
    color_treatement(path);
    detection();
    //fonction d'appel de l'ocr
    solve();
    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    if(argc == 2)
      return launch(argv[1]);
    else
      {
	gtk_init(NULL, NULL);

	builder = gtk_builder_new();
	GError* error = NULL;
	if (gtk_builder_add_from_file(builder, "window.glade", &error) == 0)
	  {
	    g_printerr("Error loading file: %s\n", error->message);
	    g_clear_error(&error);
	    return 1;
	  }
	
	mainWindow = GTK_WINDOW(gtk_builder_get_object(builder, "mainWindow"));
	stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));
	solveStack = GTK_STACK(gtk_builder_get_object(builder, "solveStack"));
	
	selectWindow = GTK_WIDGET(gtk_builder_get_object(builder, "selectWindow"));
	solveWindow = GTK_WIDGET(gtk_builder_get_object(builder, "solveWindow"));
        
	fileChooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "fileChooser"));
	buttonSelect = GTK_BUTTON(gtk_builder_get_object(builder, "buttonSelect"));
	buttonSolve = GTK_BUTTON(gtk_builder_get_object(builder, "buttonSolve"));
	buttonClose = GTK_BUTTON(gtk_builder_get_object(builder, "buttonClose"));
	buttonSolving = GTK_BUTTON(gtk_builder_get_object(builder, "buttonSolving"));
	buttonRotate = GTK_BUTTON(gtk_builder_get_object(builder, "buttonRotate"));
	spinner = GTK_SPINNER(gtk_builder_get_object(builder, "spin"));

	GtkImage* imageLogo = GTK_IMAGE(gtk_builder_get_object(builder, "logo"));
	gtk_image_set_from_file(imageLogo, "logo.png");
	imageResolved = GTK_IMAGE(gtk_builder_get_object(builder, "imageResolved"));
	imageUnresolved = GTK_IMAGE(gtk_builder_get_object(builder, "imageUnresolved"));
	path = NULL;
        char *cwd = malloc(200 * sizeof(char));
        cwd = getcwd(cwd, 200);
        pathCur = g_file_new_for_path(cwd);
        free(cwd);
	
	g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(fileChooser, "file-set", G_CALLBACK(on_choice), NULL);
	g_signal_connect(buttonSolve, "clicked", G_CALLBACK(on_stack_solve), NULL);
	g_signal_connect(buttonClose, "clicked", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(buttonSelect, "clicked", G_CALLBACK(on_stack_select), NULL);
	g_signal_connect(buttonSolving, "clicked", G_CALLBACK(on_solve), NULL);
	g_signal_connect(buttonRotate, "clicked", G_CALLBACK(on_rotate), NULL);
	
	gtk_main();
	
	return 0;
      }
}
