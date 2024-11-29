#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "image-lib.h"
#include "gd.h"


extern char* WATER_DIR;
extern char* RESIZE_DIR;
extern char* THUMB_DIR;
extern char* IMG_DIR;
extern int n_files;
extern char** files;

//extern struct thread_info /*thread_process*/;

int Processa_watermarks(char* img_dir);
int Processa_resizes(char* img_dir);
int Processa_thumbnails(char* img_dir);
void* Processa_Watermarks(void* args);
void* Processa_Resizes(void* args);
void* Processa_Thumbnails(void* args);
int Check_existing_images(char* img_file);
