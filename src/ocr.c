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

#include <gtk/gtk.h>

#include "err.h"
#include "math.h"

#include <sys/types.h>
#include <sys/stat.h>

GtkBuilder *builder;
GtkWindow *mainWindow;

GtkFileChooser *fileChooser;
GtkButton *buttonSolve;
GtkButton *buttonClose;

GFile *path;

GtkImage *imageResolved;

void on_close(GtkButton *buttonClose)
{
  gtk_window_close(mainWindow);
}

void on_solve(GtkButton *buttonSolve)
{
  if (path != NULL)
    {
      char *textpath = "given_grid/image_01.jpeg"; //g_file_get_relative_path(parent, path);
      launch(textpath);
    }

  gtk_image_set_from_file(imageResolved, "out/resolve.bmp");
}

void on_choice(GtkFileChooser *fileChooser)
{
  path = gtk_file_chooser_get_current_folder_file(fileChooser);

  char *textpath = g_file_get_path(path);
  
  gtk_image_set_from_file(imageResolved, textpath);

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
	
	GtkButton* buttonTest = GTK_BUTTON(gtk_builder_get_object(builder, "buttonTest"));
	fileChooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "fileChooser"));
	buttonSolve = GTK_BUTTON(gtk_builder_get_object(builder, "buttonSolve"));
	buttonClose = GTK_BUTTON(gtk_builder_get_object(builder, "buttonClose"));
	
	imageResolved = GTK_IMAGE(gtk_builder_get_object(builder, "imageResolved"));
	path = NULL;
	
	g_signal_connect(mainWindow, "destroy", G_CALLBACK(gtk_main_quit), NULL);
        
	g_signal_connect(fileChooser, "file-set", G_CALLBACK(on_choice), NULL);
	g_signal_connect(buttonSolve, "clicked", G_CALLBACK(on_solve), NULL);
	g_signal_connect(buttonClose, "clicked", G_CALLBACK(on_close), NULL);
	
	gtk_main();
	
	return 0;
      }
}
