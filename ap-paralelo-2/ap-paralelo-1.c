/****************************************************************************************************
 * Programação Concorrente                                                                          *
 * MEEC 22/23                                                                                       *
 *                                                                                                  *    
 * Projecto - Parte A                                                                               *                            
 *                           ap-paralelo-1.c                                                        *
 *                                                                                                  *
 * Compilação: gcc -Wall -std=c11 -v -pedantic ap-paralelo-1.c image-lib.c -g -o ap-paralelo-1 -lgd *
 *                                                                                                  *
 ***************************************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h> 
//#include "Processa_watermarks.h"
//#include "Processa_Thumbnails.h"
//#include "Processa_resizes.h"
#include "Processa_ficheiros.h"

/* the directories wher output files will be placed */
#define RESIZE_DIR "./Resize-dir/"
#define THUMB_DIR  "./Thumbnail-dir/"
#define WATER_DIR  "./Watermark-dir/"
#define PNG_LIST   "/image-list.txt"

const int n_files = 100;       // maximum number of image files to process
const int img_name_size = 100; // maximum size in chars of an image filename

typedef struct {
    int start_index;
    int n_files;
} thread_info;

/****************************************************************************************************
 * main()                                                                                           *                                                                               
 *                                                                                                  *
 * Arguments: (PNG_DIR_NAME, #THREADS)                                                              *
 * Returns: 0 in case of sucess, positive number in case of failure                                 *
 * Side-Effects: creates watermarked, watermarked thumbnail, watermarked resized copies of images   *
 *                                                                                                  *
 * Description: implementation of the parallelized serial-basic version                             *
 *              This application only works for a fixed pre-defined set of PNG files                *
 *                                                                                                  * 
 ***************************************************************************************************/

char* IMG_DIR = 0;
int n_threads = 0;
int main(int argc, char* argv[]) { 
    printf("argv[0]: %s\n", argv[0]); // Dbg purpose; to delete
    printf("argv[1]: %s\n", argv[1]); // Dbg purpose; to delete
    printf("argv[2]: %s\n", argv[2]); // Dbg purpose; to delete
    (argc == 3 ? (n_threads = atoi(argv[2]), IMG_DIR = argv[1]) : (fprintf(stderr, "No image list or no number of threades given. Exiting.\n"), exit(-1)));
    if (create_directory(IMG_DIR) == 0){
		fprintf(stderr, "No such directory exists: '%s'. Exiting.\n", RESIZE_DIR);
		exit(-1);
	}
    if (!n_threads) n_threads = 1;
    if (n_threads < 0) n_threads*= -1;
    printf("Number of threads will be: %d\n", n_threads); // Dbg purpose; to delete 
    printf("Image directory is: %s\n", IMG_DIR);         // Dbg purpose; to delete 
    if (create_directory(RESIZE_DIR) == 0) {
		fprintf(stderr, "Impossible to create '%s' directory. Exiting.\n", RESIZE_DIR);
		exit(-1);
	}
	if (create_directory(THUMB_DIR) == 0) {
		fprintf(stderr, "Impossible to create '%s' directory. Exiting.\n", THUMB_DIR);
		exit(-1);
	}
	if (create_directory(WATER_DIR) == 0) {
		fprintf(stderr, "Impossible to create '%s' directory. Exiting.\n", WATER_DIR);
		exit(-1);
	}
    char* img_name[n_files];
    int n_img = 0; // number of images to process
    char** files = 0;  // array of image names to process
    FILE* fp = 0;
    strcat(IMG_DIR, PNG_LIST);
    printf("Reading '%s'.\n", IMG_DIR);
    fp = fopen(IMG_DIR, "rb");
    if (!fp) {
        fprintf(stderr, "File %s can't be opened or does not exist.\n", PNG_LIST);
        exit(-1);
    }
    char* png_file = ".png";
    while (fgets((char*)img_name, img_name_size, fp) != 0) {
        char* img_file = strtok((char*)img_name, "\n");
        if (!img_file) break;
        //printf("img_file: %s\n", img_file);  // Dbg purpose; to delete
        char* file_format = strstr(img_file, png_file);
        //printf("strstr: %s\n", file_format);  // Dbg purpose; to delete
        if (!strcmp(file_format, png_file)) {
            files = &img_file;
            puts(*files); // Dbg purpose; to delete 
            files++;
            n_img++;
        }
    }
    printf("n_img: %d\n", n_img); // Dbg purpose; to delete
    div_t files_separation = div(n_img, n_threads);
    thread_info threads[n_threads];
    int imgs_per_thread = files_separation.quot;
    for (int i = 0 ; i < imgs_per_thread - 1; i++) threads[i].n_files = (int)imgs_per_thread;
    threads[imgs_per_thread - 1].n_files = (int)files_separation.rem;
    pthread_t thread_id[n_threads];
    /*void* thread_ret;*/
    /*int ret_val;*/
    //char* thread_args =  calloc(n, sizeof(thread_info));;
    for (int i = 0; i < n_threads; i++) {
        //sprintf(thread_args, "%d, %d, %p", i, files_per_thread[i], (void*)files);
        printf("Thread %d creation\n", i + 1); // Dbg purpose; to delete
        pthread_create(&thread_id[i], 0, Processa_ficheiros, &threads[i]);
        //realloc...
    }
    for (int i = 0; i < n_threads; i++) {
        printf("Thread %d junction\n", i + 1); // Dbg purpose; to delete
        pthread_join(thread_id[i], 0/*&thread_ret*/);
    }
    fclose(fp);
    fp = 0;
    files = 0; 
}
