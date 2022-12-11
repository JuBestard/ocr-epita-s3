#ifndef OCR_H
#define OCR_H

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
#include <SDL2/SDL_surface.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "err.h"
#include "math.h"
#include <sys/types.h>
#include <sys/stat.h>

int launch(char* path_to_image);

#endif
