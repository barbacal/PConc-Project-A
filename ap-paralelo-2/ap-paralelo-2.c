/*************************************************************************************************************************
 * Programação Concorrente                                                                                               *
 * MEEC 22/23                                                                                                            *
 *                                                                                                                       *    
 * Projecto - Parte A                                                                                                    *                            
 *                           ap-paralelo-1.c                                                                             *
 *                                                                                                                       *
 * Compilação: gcc -Wall -std=c11 -v -pedantic ap-paralelo-1.c Processa_ficheiros.c image-lib.c -g -o ap-paralelo-1 -lgd *
 *                                                                                                                       *
 ************************************************************************************************************************/

#include "ap-paralelo-2.h"

/****************************************************************************************************
 * main()                                                                                           *                                                                               
 *                                                                                                  *
 * Arguments: (PNG_DIR_NAME, #THREADS)                                                              *
 * Returns: 0 in case of sucess, -1 in case of failure                                 *
 * Side-Effects: creates watermarked, watermarked thumbnail, watermarked resized copies of images   *
 *                                                                                                  *
 * Description: implementation of the parallelized serial-basic version                             *
 *              This application only works for a fixed pre-defined set of PNG files                *
 *                                                                                                  * 
 ***************************************************************************************************/

int main(int argc, char* argv[]) {
    Check_Input_Args(argc, argv);
    Check_Dirs();
    files = Read_Files_List();
    puts("files in main:"); for (int i = 0; i < n_img; i++) puts(files[i]); // Dbg purpose; to delete
    Parallelize();
    FreeAlloc();
    exit(0);
}

void* Parallelize() {
    /*void* thread_ret;*/
    /*int ret_val;*/
    pthread_t threads_id[n_threads];
    /*Processa_Watermarks*/
    char* arg = "IMG_DIR"; 
    puts("Processa_Watermarks");
    printf("Thread %d creation\n", 1); // Dbg purpose; to delete
    pthread_create(&threads_id[0], 0, Processa_Watermarks, &arg);
    printf("Thread %d junction\n", 1); // Dbg purpose; to delete
    pthread_join(threads_id[0], 0/*&thread_ret*/);
    /*Processa_Resizes*/
    arg = "WATER_DIR";
    puts("Processa_Resizes");
    printf("Thread %d creation\n", 2); // Dbg purpose; to delete
    pthread_create(&threads_id[1], 0, Processa_Resizes, arg);
    printf("Thread %d junction\n", 2); // Dbg purpose; to delete
    pthread_join(threads_id[1], 0/*&thread_ret*/);
    /*Processa_Resizes*/
    puts("Processa_Thumbnails");
    printf("Thread %d creation\n", 3); // Dbg purpose; to delete
    pthread_create(&threads_id[2], 0, Processa_Thumbnails, arg);
    printf("Thread %d junction\n",3); // Dbg purpose; to delete
    pthread_join(threads_id[2], 0/*&thread_ret*/);
    return (void*)0;    
}

void* Check_Dirs() {
    if (!create_directory(IMG_DIR)){
		fprintf(stderr, "No such directory exists: '%s'. Exiting\n", IMG_DIR);
		exit(-1);
	}
    //printf("Image directory is: %s\n", IMG_DIR);         // Dbg purpose; to delete 
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
    return (void*)0;
}

void* Check_Input_Args(int argc, char* argv[]) {
    printf("argv[0]: %s\n", argv[0]); // Dbg purpose; to delete
    printf("argv[1]: %s\n", argv[1]); // Dbg purpose; to delete
    (argc == 2) ? (void)(IMG_DIR = argv[1]) : (fprintf(stderr, "No image directory\n"), puts("Usage example: 'ap-paralelo-2 Image-dir'. Exiting."), exit(-1));
    return (void*)0;
    printf("Image directory is: %s\n", IMG_DIR);          // Dbg purpose; to delete 
}

char** Read_Files_List() {
     FILE* fp = 0;
    const int name_size = 100; // maximum size in chars of an image filename
    char img_name[name_size];
    char* img_file = 0;
    printf("Reading '%s'.\n", PNG_LIST);
    fp = fopen(PNG_LIST, "rb");
    if (!fp) {
        fprintf(stderr, "File %s can't be opened or does not exist.\n", PNG_LIST);
        exit(-1);
    }
    while (fgets(img_name, name_size, fp) != 0) {
        img_file = strtok((char*)img_name, "\n");
        if (!img_file) break;
        char* file_format = strstr(img_file, png_file);
        if (!strcmp(file_format, png_file)) n_img++;
    }
    //printf("n_img: %d\n", n_img); // Dbg purpose; to delete
    rewind(fp);
    char** files = (char**)calloc(n_img, sizeof(char*));
        if (!files) {
        puts("Cannot process images. Aborting.");
        exit(-1);
    }
    int i = 0;
    while (fgets(img_name, name_size, fp) != 0) { //perhaps 'fscanf' is a better option: fscanf(fp, "%s", img_name); 
        img_file = strtok((char*)img_name, "\n");
        if (!img_file) break;
        //printf("img_file: %s\n", img_file);  // Dbg purpose; to delete
        char* file_format = strstr(img_file, png_file);
        //printf("strstr: %s\n", file_format);  // Dbg purpose; to delete
        if (!strcmp(file_format, png_file)) {
            files[i] = (char*)malloc((strlen(img_name) + 1) * sizeof(char));
            if (!files[i]) {
                printf("Cannot process %s. Skipping this one.\n", files[i]);
                continue;
            } 
            strcpy(files[i], img_file);
            //puts(files[i]); // Dbg purpose; to delete 
            i++;
        }
    }
    fclose(fp);
    fp = 0;
    return files;
}

void* FreeAlloc() {
    free(files);
    return (void*)0;
}
