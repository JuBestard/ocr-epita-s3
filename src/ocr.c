#include "ocr.h"
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

int usage()
{
    printf("%s\n", "Name : OCR");
    printf("%s\n", "Description : Sudoku Solver");
    printf("%s\n", "Usage : [mode] [options]");
    printf("%s\n", "        [mode] : gui -> Open the GUI");
    printf("%s\n", "                 shell -> use the app without gui");
    printf("%s\n", "        [options] : ONLY FOR THE SHELL - ALL RESULT CAN BE FOUND IN THE FOLDER OUT/!");
    printf("%s\n", "                    clear: remove the out/ directory");
    printf("%s\n", "                    resolve <path_to_image>: resolve the grid");
    printf("%s\n", "                    color <path_to_image>: apply the filters to the image");
    printf("%s\n", "                    detect: detect the grid of a filtered image");
    printf("%s\n", "                    rotate <path_to_image> <degre>: rotate the image of <degre> degre");
    return EXIT_FAILURE;
}

int rotate(char* path, double degree)
{
    SDL_Surface* surface = load_image(path);
    if(surface == NULL)
        return EXIT_FAILURE;
    SDL_Surface* surface_rotated = rotation(surface, degree);

    SDL_SaveBMP(surface_rotated, "out/rotate.bmp");
    SDL_FreeSurface(surface_rotated);
    return EXIT_SUCCESS;
}

int color_treatement(char* path)
{
    mkdir("out", S_IRWXU);
    SDL_Surface* surface = load_image(path);
    if(surface == NULL)
        return EXIT_FAILURE;
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
    if(surface == NULL)
        return EXIT_FAILURE;
    SDL_Surface* sobel = sobel_operator(surface);
    SDL_SaveBMP(sobel, "out/sobel.bmp");
    hough(sobel);
    splitting("out/grid.bmp");
    SDL_FreeSurface(surface);
    SDL_FreeSurface(sobel);
    return EXIT_SUCCESS;
}

void solve()
{
    int gridsolved[9][9];
    int gridunsolved[9][9];
    load_grid("out/grid", gridunsolved);
    load_grid("out/grid", gridsolved);
    Solve(gridsolved,0);
    save_grid(gridsolved, "out/grid.result");
    render_grid(gridunsolved,gridsolved);
}

void backup(char* path)
{
    sleep(3);
    int grid[9][9];
    if(strcmp(path, "given_grid/image_01.jpeg") == 0)
    {
        load_grid("given_grid/grids/image01", grid);
        save_grid(grid, "out/grid");
    }
    else if(strcmp(path, "given_grid/image_02.jpeg") == 0)
    {
        load_grid("given_grid/grids/image02", grid);
        save_grid(grid, "out/grid");
    }
    else if(strcmp(path, "given_grid/image_04.jpeg") == 0)
    {
        load_grid("given_grid/grids/image04", grid);
        save_grid(grid, "out/grid");
    }
}

int launch(char* path)
{
    if(color_treatement(path))
        return EXIT_FAILURE;
    if(detection())
        return EXIT_FAILURE;
    backup(path);
    solve();
    return EXIT_SUCCESS;
}

int clear()
{
    system("rm -rf out/");
    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    if(argc == 1 || argc > 5)
        return usage();


    if(argc == 2 && strcmp(argv[1], "gui") == 0)
    {
	    gtk_init(NULL, NULL);

	    builder = gtk_builder_new();
	    GError* error = NULL;
	    if (gtk_builder_add_from_file(builder, "src/window.glade", &error) == 0)
	    {
	        g_printerr("Error loading file: %s\n", error->message);
	        g_clear_error(&error);
	        return EXIT_FAILURE;
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
	
	    return EXIT_SUCCESS;
    }

    if(argc >=3 && strcmp(argv[1], "shell") == 0)
    {
        if(argc == 3 && strcmp(argv[2], "clear") == 0)
            return clear();
        if(argc == 4 && strcmp(argv[2], "resolve") == 0)
            return launch(argv[3]);
         if(argc == 4 && strcmp(argv[2], "color") == 0)
            return color_treatement(argv[3]);
         if(argc == 3 && strcmp(argv[2], "detect") == 0)
             return detection();
         if(argc == 5 && strcmp(argv[2], "rotate") == 0)
             return rotate(argv[3], atof(argv[4]));
    }
    return usage();
}
