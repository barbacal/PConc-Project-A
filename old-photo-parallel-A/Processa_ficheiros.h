#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "image-lib.h"
#include <gd.h>



extern char* CONTRAST_DIR;
extern char* SMOOTH_DIR;
extern char* TEXTURE_DIR;
extern char* SEPIA_DIR;
extern char* IMG_DIR;
extern char** files;

//extern struct thread_info /*thread_process*/;

int Processa_contrast(char** files, int n_files, int index);
int Processa_smooth(char** files, int n_files, int index);
int Processa_texture(char** files, int n_files, int index);
int Processa_sepia(char** files, int n_files, int index);
int Check_existing_image(char* img_file);