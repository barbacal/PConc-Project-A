#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "image-lib.h"
#include "Processa_ficheiros.h"
 
/* The directories where output files will be placed */
char* RESIZE_DIR = "./Resize-dir/";
char* THUMB_DIR  = "./Thumbnail-dir/";
char* WATER_DIR  = "./Watermark-dir/";

/* The images' filename list*/
char* PNG_LIST =   "image-list.txt";

/* The directory where input files will be picked */
char* IMG_DIR = 0;

char* png_file = ".png";          // type of image format (in this case PNG)
int n_img = 0;                    // number of images to process
char** files = 0;                 // array of images to process
int n_files = 0;                  // number of files to process
const int n_threads = 0;          // required number of threads for ap-paralelo-2

void* Check_Input_Args(int argc, char* argv[]);
void* Check_Dirs();
char** Read_Files_List();
void* Parallelize();
void* FreeAlloc();
