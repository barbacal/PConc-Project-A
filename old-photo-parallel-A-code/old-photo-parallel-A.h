#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//#include <string.h>
//#include "image-lib.h"
#include <ctype.h>
#include "Processa_ficheiros.h"
#include <bits/pthreadtypes.h>
//#include <linux/time.h> //CLOCK_MONOTONIC not available in my Ubuntu PC
#include <time.h>
//#include <sys/time.h>
//#include <errno.h>

struct timespec start_time_total, end_time_total;
struct timespec start_time_ser, end_time_ser;
struct timespec* start_time_par;
struct timespec* end_time_par;

/* File with timings: total, serial, parallel per thread*/
char* timing_file = 0;

/* The image ordering option */
char* OPTION = 0;

/* The directories where output files will be placed */
char* OLD_PHOTO_PAR_A = "/old_photo_PAR_A/";
char* CONTRAST_DIR = "/old_photo_PAR_A/Contrast-dir/";
char* SMOOTH_DIR = "/old_photo_PAR_A/Smooth-dir/";
char* TEXTURE_DIR  = "/old_photo_PAR_A/Texture-dir/";
char* SEPIA_DIR  = "/old_photo_PAR_A/Sepia-dir/";

/* The images' filename list*/
char* IMG_LIST =   "image-list.txt";

/* The directory where input files will be picked */
char* IMG_DIR = 0;

bool finish = false;

pthread_barrier_t bar; // barrier for threads

int n_threads = 1;                // Default threads (besides main)
const char* jpg_file = ".jpg\0";          // type of image format (in this case JPG)
const char* jpeg_file = ".jpeg\0";        // type of image format (in this case JPEG)
const char* png_file = ".png\0";          // type of image format (in this case PNG)
char* image_format = 0;            // type of image format 
int n_img = 0;                    // number of images to process
char** files = 0;                 // array of images to process

typedef struct thread_info {
    int search_index; // in the array of image names to process by thread
    int n_files;      // number of images to process by thread
    pthread_barrier_t* barrier; // experimental for self learning
    char** files;	
} thread_info;

void* Check_Input_Args(int argc, char* argv[]);
void* Check_Dirs();
char** Read_Files_List();
thread_info* Make_thread_info_array();
void* Processa_threads(void* args);
void* Parallelize_Serial(thread_info* threads);
void* FreeAlloc();
bool Check_for_Images();
_Bool Check_for_Extension(const char* filename, const char* ext);
void* OrderFiles();
int Compare_Size(const void* a, const void* b);
int Compare_Name(const void* a, const void* b);
void* FinishTiming();
void* StartTiming();
void* FinishTimingSerial();
void* GetParallelTiming(struct timespec* start, struct timespec* end, long int thr_id);
