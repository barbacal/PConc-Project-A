#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
//#include "image-lib.h"
#include "Processa_ficheiros.h"


struct timespec start_time_par, end_time_par;

/* The image ordering option */
char* OPTION = 0;

/* The directories where output files will be placed */
char* CONTRAST_DIR = "/old_photo_PAR_A/Contrast-dir/";//??CHECK!!
char* SMOOTH_DIR = "/old_photo_PAR_A/Smooth-dir/";//??CHECK!!
char* TEXTURE_DIR  = "/old_photo_PAR_A/Texture-dir/";//??CHECK!!
char* SEPIA_DIR  = "/old_photo_PAR_A/Sepia-dir/";//??CHECK!!

/* The images' filename list*/
char* IMG_LIST =   "image-list.txt";

/* The directory where input files will be picked */
char* IMG_DIR = 0;

int n_threads = 0;                // Default threads (besides main)
char* jpg_file = ".jpg";          // type of image format (in this case JPG)
char* jpeg_file = ".jpeg";        // type of image format (in this case JPEG)
char* png_file = ".png";          // type of image format (in this case PNG)
char* image_format = 0;            // type of image format 
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
bool Check_for_Images();
bool Check_for_Extension(char* filename, char* ext);
void* OrderFiles();
int Compare_Size(const void* a, const void* b);
int Compare_Name(const void* a, const void* b);
void* FinishTiming();
void* StartTiming();