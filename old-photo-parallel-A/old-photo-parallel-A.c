/*************************************************************************************************************************************************
 * Programação Concorrente                                                                                                                       *
 * MEEC 22/23                                                                                                                                    *
 *                                                                                                                                               *    
 * Projecto - Parte A                                                                                                                            *                            
 *                           old-photo-parallel-A.c                                                                                              *
 *                                                                                                                                               *
 * Compilação: gcc -Wall -std=c11 -v -pedantic old-photo-parallel-A.c Processa_ficheiros.c image-lib.c -g -o old-photo-parallel-A -lpthread -lgd *
 *                                                                                                                                               *
 ************************************************************************************************************************************************/

#include "old-photo-parallel-A.h"

/****************************************************************************************************
 * main()                                                                                           *                                                                               
 *                                                                                                  *
 * Arguments: (PNG_DIR_NAME, #THREADS, ORDERING_OPTION)                                             *
 * Returns: 0 in case of sucess, -1 in case of failure                                              *
 * Side-Effects: creates contrast, smooth, sepia and texture copies of images                       *
 *                                                                                                  *
 * Description: implementation of the parallelized old-photo-serial version                         *
 *              This application only works for a fixed pre-defined set of PNG, JPEG oo JPG files   *
 *                                                                                                  * 
 ***************************************************************************************************/

int main(int argc, char* argv[]) {
    StartTiming();
    Check_Inputt_Args(argc, argv);
    Check_Dirs();
    files = Read_Files_List();
    //puts("files in main:"); for (int i = 0; i < n_img; i++) puts(files[i]); // Dbg purpose; to delete
    OrderFiles();
    thread_info* threads = Make_thread_info_array();
    Parallelize_Serial(threads);
    FreeAlloc();
    FinishTiming();
    puts("Done.");
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
        Processa_contrast(images_copy, thread_process.n_files, thread_process.search_index);
        Processa_smooth(images_copy, thread_process.n_files, thread_process.search_index);
        Processa_texture(images_copy, thread_process.n_files, thread_process.search_index);
        Processa_sepia(images_copy, thread_process.n_files, thread_process.search_index);
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
    printf("Image directory is: %s\n", IMG_DIR);         // Dbg purpose; to delete 
    CONTRAST_DIR = strcat(IMG_DIR, CONTRAST_DIR);
    if (create_directory(CONTRAST_DIR) == 0) {
		fprintf(stderr, "Impossible to create '%s' directory. Exiting.\n", CONTRAST_DIR);
		exit(-1);
	}
    SMOOTH_DIR = strcat(IMG_DIR, SMOOTH_DIR);
    if (create_directory(SMOOTH_DIR) == 0) {
		fprintf(stderr, "Impossible to create '%s' directory. Exiting.\n", SMOOTH_DIR);
		exit(-1);
	}
    TEXTURE_DIR = strcat(IMG_DIR, TEXTURE_DIR);
	if (create_directory(TEXTURE_DIR) == 0) {
		fprintf(stderr, "Impossible to create '%s' directory. Exiting.\n", TEXTURE_DIR);
		exit(-1);
        
	}
    SEPIA_DIR = strcat(IMG_DIR, SEPIA_DIR);
	if (create_directory(SEPIA_DIR) == 0) {
		fprintf(stderr, "Impossible to create '%s' directory. Exiting.\n", SEPIA_DIR);
		exit(-1);
	}
    return (void*)0;
}

void* Check_Input_Args(int argc, char* argv[]) {
    //printf("argv[0]: %s\n", argv[0]); // Dbg purpose; to delete
    //printf("argv[1]: %s\n", argv[1]); // Dbg purpose; to delete
    //printf("argv[2]: %s\n", argv[2]); // Dbg purpose; to delete
    const char* help = "Wrong calling. Usage example with 4 threads: './old-photo-parallel-A ./dir-1 4 -size (or -name)'. Exiting.";
    (argc < 4) ? (puts(help), exit(-1)) : (n_threads = strtol(argv[2], NULL, 100));
    OPTION = argv[3];
    if (!n_threads) {
        puts("Invalid positive number.");
        puts(help);
        exit(-1);
    } else if ((OPTION != "-size") && (OPTION != "-name")) {
        puts("Invalid image ordering option.");
        puts(help);
        exit(-1);
    }
    IMG_DIR = argv[1];
    printf("Number of threads will be: %d\n", n_threads); // Dbg purpose; to delete 
    printf("Image directory is: %s\n", IMG_DIR);          // Dbg purpose; to delete 
}

char** Read_Files_List() {
    bool isFileList = true
    FILE* fp = 0;
    const int name_size = 100; // maximum size in chars of an image filename
    char img_name[name_size];
    char* img_file = 0;
    printf("Reading '%s'.\n", IMG_LIST);
    fp = fopen(IMG_LIST, "rb");
    if (!fp) {
        fprintf(stderr, "File %s can't be opened or does not exist.\n", IMG_LIST);
        isFileList = false;
        puts("Checking for images in image folder.");
        if (!Check_for_Images()){
            puts("No images found of formats .jpg, .jpg or .png. Exiting.");
            exit(-1);
        }
    }
    if (isFileList) {
        Check_for_Images();
        while (fgets(img_name, name_size, fp) != 0) {
            img_file = strtok((char*)img_name, "\n");
            if (!img_file) break;
            char* file_format = strstr(img_file, image_format);
            if (file_format != NULL && !strcmp(file_format, image_format)) n_img++;
        }
        printf("n_img: %d\n", n_img); // Dbg purpose; to delete
        rewind(fp);
        char** files = (char**)calloc(n_img, sizeof(char*));
        if (!files) {
            puts("Cannot process images from list. Aborting.");
            exit(-1);
        }
        int i = 0;
        while (fgets(img_name, name_size, fp) != 0) { //perhaps 'fscanf' is a better option: fscanf(fp, "%s", img_name); 
            img_file = strtok((char*)img_name, "\n");
            if (!img_file) break;
            //printf("img_file: %s\n", img_file);  // Dbg purpose; to delete
            char* file_format = strstr(img_file, image_format);
            //printf("strstr: %s\n", file_format);  // Dbg purpose; to delete
            if (file_format != NULL && !strcmp(file_format, image_format)) {
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
    } else {
        DIR* dir;
        dir = opendir(IMG_DIR);
        if (!dir) {
            fprintf(stderr, "Directory %s does not exist. Exiting.\n", IMG_DIR);
            exit(-1);
        }
        struct dirent* entry;
        int i = 0;
        while ((entry = readdir(dir)) != 0) {
            if (entry->d_type == DT_REG) {
                if (Check_for_Extension((img_name = entry->d_name), image_format)) {
                    img_file = strtok((char*)img_name, "\n")
                    printf("Found image of format: %s\n", image_format);
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
        }
        closedir(dir);
    }
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
    if (imgs_per_thread == 0) {
        n_threads = n_img;
        printf("Less images than threads. Processing %d threads, one per image.\n", n_threads);
        for (int i = 0 ; i < n_threads; i++) {
            threads[i].n_files = 1;
            threads[i].search_index = i;
        }
    } else if (imgs_per_last_thread != 0) {
        threads[n_threads - 1].n_files = imgs_per_last_thread;
        threads[n_threads - 1].search_index = imgs_per_thread * (n_threads - 1);
        printf("Make..._array n_files = %d\n", threads[n_threads - 1].n_files);
        printf("Make..._array index = %d\n", threads[n_threads - 1].search_index);
    } else {
        threads[n_threads - 1].n_files = imgs_per_thread;
        threads[n_threads - 1].search_index = imgs_per_thread * (n_threads - 1);
        printf("Make..._array n_files = %d\n", threads[n_threads - 1].n_files);
        printf("Make..._array index = %d\n", threads[n_threads - 1].search_index);
    }
    return threads;
}

void* FreeAlloc() {
    free(files);
    return (void*)0;
}

bool Check_for_Images() {
    bool res = false;
    int n_formats = 2;
    char* file_format[3] = {png_file, jpg_file, jpeg_file}; //Assumes either PNG, JPG or JPEG or empty folder
    DIR* dir;
 loop:  dir = opendir(IMG_DIR);
    if (!dir) {
        fprintf(stderr, "Directory %s does not exist. Exiting.\n", IMG_DIR);
        exit(-1);
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != 0) {
        if (entry->d_type == DT_REG) {
            if (Check_for_Extension(entry->d_name, file_format[n_formats])) {
                res = true;
                image_format = file_format[n_formats];
                printf("Found image(s) of format: %s\n", file_format[i]);
                break;
            }
        }
    }
    closedir(dir);
    if(!res && !n_formats) {
        n_formats--;
        goto loop;
    }
    return res;
}
bool Check_for_Extension(const char* filename, char* ext) {
    size_t filename_len = strlen(filename);
    size_t ext_len = strlen(ext);
    const char *file_ext = filename + filename_len - ext_len;
    while (*file_ext && *ext) {
        if (tolower(*file_ext++) != tolower(*ext++)) {
            return false;
        }
    }
    return true;
}

void* OrderFiles() {
    if (OPTION == "-size") {
        qsort(files, n_img, sizeof(char*), Compare_Size);
    } else {
        qsort(files, n_img, sizeof(char*), Compare_Name);
    }
    return (void*)0;
}

int Compare_Size(const void* a, const void* b) {
    struct stat st_a, st_b;
    stat((char*)a, &st_a);
    stat((char*)b, &st_b);
    return (st_a.st_size - st_b.st_size);
}

int Compare_Name(const void* a, const void* b) {
    return strcmp((char*)a, (char*)b);
}

void* StartTiming() {
	clock_gettime(CLOCK_MONOTONIC, &start_time_par);
}

void* FinishTiming() {
    clock_gettime(CLOCK_MONOTONIC, &end_time_par);
    struct timespec par_time = diff_timespec(&end_time_par, &start_time_par);
    FILE *file;
    char* timing_file, timing = (char*)malloc(100 * sizeof(char));
    timing_file = IMG_DIR;
    timing_file = strcat(timing_file, "<");
    char* str_n_threads = (char*)malloc(3 * sizeof(char));
    sprintf(str_n_threads, "%d", n_threads);
    timing_file = strcat(timing_file, str_n_threads);
    sprintf(timing_file, "%s>%s.txt", timing_file, OPTION);
    file = fopen(timing_file, "w");
    if (!fp) {
        fprintf(stderr, "File %s can't be created.\n", timing_file);
        exit(-1);
    }
    timing = sprintf(timing, "\tpar \t %10jd.%09ld\n", par_time.tv_sec, par_time.tv_nsec);
    if (fputs(timing, fp) == EOF) {
        fprintf(stderr, "Error on writing in file %s.\n", timing_file);
        fclose(file);
        fp = 0;
        exit(-1);
    }
    close(fp);
    fp = 0;
}