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

#include "ap-paralelo-1.h"

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
    Check_Inputt_Args(argc, argv);
    Check_Dirs();
    files = Read_Files_List();
    puts("files in main:"); for (int i = 0; i < n_img; i++) puts(files[i]); // Dbg purpose; to delete
    thread_info* threads = Make_thread_info_array();
    Parallelize_Serial(threads);
    FreeAlloc();
    exit(0);
}

void* Parallelize_Serial(thread_info* threads) {
    pthread_t thread_id[n_threads];
    /*void* thread_ret;*/
    /*int ret_val;*/
    for (int i = 0; i < n_threads; i++) {
        printf("Thread %d creation\n", i + 1); // Dbg purpose; to delete
        pthread_create(&thread_id[i], 0, Processa_threads, &threads[i]);
    }
    for (int i = 0; i < n_threads; i++) {
        printf("Thread %d junction\n", i + 1); // Dbg purpose; to delete
        pthread_join(thread_id[i], 0/*&thread_ret*/);
    }
    //threads = 0;
    return (void*)0;    
}

void* Processa_threads(void* args) {
    thread_info thread_process = *(thread_info*) args;
    printf("In 'Processa_threads' \nthread id = %ld\n", pthread_self());
    printf("n_files = %d; ", (int)thread_process.n_files);
    printf("search_index = %d\n", (int)thread_process.search_index);
    char** images_copy = (char**)malloc(sizeof files);
    images_copy = files;
    for (int i = 0; i < thread_process.n_files; i++) {
        printf("start iteration %d\n", i);
        Processa_watermarks(images_copy, thread_process.n_files, thread_process.search_index);
        //Processa_resizes(images_copy, thread_process.n_files, thread_process.search_index);
        //Processa_thumbnails(images_copy, thread_process.n_files, thread_process.search_index);
        printf("end iteration %d\n", i);
    }
    puts("out");
    return (void*)0;
}

void* Check_Dirs() {
    if (!create_directory(IMG_DIR)){
		fprintf(stderr, "No such directory exists: '%s'. Exiting\n", IMG_DIR);
		exit(-1);
	}
    if (!n_threads) {
        puts("No threads given, assuming one thread then");
        n_threads = 1;
    }
    if (n_threads < 0) {
        n_threads*= -1;
        printf("Only a positive number of threads, threads will be %d\n", n_threads); 
    }
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
    return (void*)0;
}

void* Check_Input_Args(int argc, char* argv[]) {
    //printf("argv[0]: %s\n", argv[0]); // Dbg purpose; to delete
    //printf("argv[1]: %s\n", argv[1]); // Dbg purpose; to delete
    //printf("argv[2]: %s\n", argv[2]); // Dbg purpose; to delete
    (argc == 3) ? (void)(n_threads = atoi(argv[2]), IMG_DIR = argv[1]) : (fprintf(stderr, "No image directory or no number of threades given.\n"), puts("Usage example: 'ap-paralelo-1 Image-dir 4'. Exiting."), exit(-1));
    return (void*)0;
    if (!n_threads) n_threads = 1;
    if (n_threads < 0) n_threads*= -1;
    printf("Number of threads will be: %d\n", n_threads); // Dbg purpose; to delete 
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
    printf("n_img: %d\n", n_img); // Dbg purpose; to delete
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

thread_info* Make_thread_info_array() {
    printf("Make..._array: n_threads = %d\n", n_threads);
    printf("Make..._array: n_img = %d\n", n_img);
    thread_info* threads = (thread_info*)calloc((size_t)n_threads, sizeof(thread_info));
    div_t files_separation = div(n_img, n_threads);
    int imgs_per_thread = (int)files_separation.quot;
    int imgs_per_last_thread = (int)files_separation.rem;
    for (int i = 0 ; i < n_threads - 1; i++) {
        threads[i].n_files = imgs_per_thread;
        threads[i].search_index = imgs_per_thread * i;
        printf("Make..._array: n_files = %d\n", threads[i].n_files);
        printf("Make..._array: index = %d\n", threads[i].search_index);
    }
    threads[n_threads - 1].n_files = imgs_per_last_thread;
    threads[n_threads - 1].search_index = imgs_per_thread * (imgs_per_thread - 1);
    printf("Make..._array n_files = %d\n", threads[n_threads - 1].n_files);
    printf("Make..._array index = %d\n", threads[n_threads - 1].search_index);
    return threads;
}

void* FreeAlloc() {
    free(files);
    return (void*)0;
}