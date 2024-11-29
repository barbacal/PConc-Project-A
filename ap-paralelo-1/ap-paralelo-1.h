#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "image-lib.h"
#include "Processa_ficheiros.h"
//#include "Processa_watermarks.h"
//#include "Processa_Thumbnails.h"
//#include "Processa_resizes.h"
 
/* The directories where output files will be placed */
char* RESIZE_DIR = "./Resize-dir/";
char* THUMB_DIR  = "./Thumbnail-dir/";
char* WATER_DIR  = "./Watermark-dir/";

/* The images' filename list*/
char* PNG_LIST =   "image-list.txt";

/* The directory where input files will be picked */
char* IMG_DIR = 0;

int n_threads = 1;                // Minimum threads (besides main)
char* png_file = ".png";          // type of image format (in this case PNG)
int n_img = 0;                    // number of images to process
char** files = 0;                 // array of images to process

typedef struct thread_info {
    int search_index; // in the array of image names to process by thread
    int n_files;      // number of images to process by thread
} thread_info;

void* Check_Input_Args(int argc, char* argv[]);
void* Check_Dirs();
char** Read_Files_List();
thread_info* Make_thread_info_array();
void* Processa_threads(void* args);
void* Parallelize_Serial(thread_info* threads);
void* FreeAlloc();