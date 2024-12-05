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
    Check_Input_Args(argc, argv);
    Check_Dirs();
    files = Read_Files_List();
    puts("files in main:"); for (int i = 0; i < n_img; i++) puts(files[i]); // Dbg purpose; to delete
    OrderFiles();
    thread_info* threads = Make_thread_info_array();
    FinishTimingSerial();
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
        clock_gettime(CLOCK_REALTIME, &start_time_par);
        printf("%dth thread creation.\n", i + 1); // Dbg purpose; to delete
        pthread_create(&thread_id[i], 0, Processa_threads, &threads[i]);
    }
    for (int i = 0; i < n_threads; i++) {
        printf("%dth thread junction.\n", i + 1); // Dbg purpose; to delete
        long int thr_id = pthread_self();
        pthread_join(thread_id[i], 0/*&thread_ret*/);
        clock_gettime(CLOCK_REALTIME, &start_time_par);
        GetParallelTiming(&start_time_par, &end_time_par, thr_id);    
    }
    //threads = 0;
    return (void*)0;    
}

void* Processa_threads(void* args) {
    thread_info thread_process = *(thread_info*) args;
    printf("In 'Processa_threads' \nthread id = %ld\n", pthread_self());
    printf("n_files = %d; ", (int)thread_process.n_files);
    printf("search_index = %d\n", (int)thread_process.search_index);
    //char** images_copy = (char**)malloc(sizeof files);
    //images_copy = files;
    for (int i = 0; i < thread_process.n_files; i++) {
        printf("start iteration %d\n", i);
        Processa_contrast( thread_process.n_files, thread_process.search_index);
        Processa_smooth(thread_process.n_files, thread_process.search_index);
        Processa_texture( thread_process.n_files, thread_process.search_index);
        Processa_sepia(thread_process.n_files, thread_process.search_index);
        printf("end iteration %d\n", i);
    }
    puts("out");
    return (void*)0;
}

void* Check_Dirs() {
    if (create_directory(IMG_DIR) == 0){
		fprintf(stderr, "'%s' does not exist. Exiting.\n", IMG_DIR);
        rmdir(IMG_DIR);
        exit(-1);
    }
    char* dir;
    int res;
    //IMG_DIR = strtok((char*)IMG_DIR, "/");;
    size_t img_dir_len = strlen(IMG_DIR);
    dir = (char*)malloc((img_dir_len + strlen(CONTRAST_DIR) + 1) * sizeof(char));
    sprintf(dir, "%s%s", IMG_DIR, CONTRAST_DIR);
     
    CONTRAST_DIR = dir;
    //printf("Contrast directory is: %s\n", CONTRAST_DIR);         // Dbg purpose; to delete 
    if ((res = create_directory(CONTRAST_DIR)) == 0) {
		printf("'%s' created.\n", CONTRAST_DIR);
	} else if (res == 1) fprintf(stderr, "'%s' already exists.\n", CONTRAST_DIR);
    dir = (char*)malloc((img_dir_len + strlen(SMOOTH_DIR) + 1) * sizeof(char));
    sprintf(dir, "%s%s", IMG_DIR, SMOOTH_DIR);
     
    SMOOTH_DIR = dir;
    if ((res = create_directory(SMOOTH_DIR)) == 0) {
		printf("'%s' created.\n", SMOOTH_DIR);
	} if (res == 1) fprintf(stderr, "'%s' already exists.\n", SMOOTH_DIR);
    dir = (char*)malloc((img_dir_len + strlen(TEXTURE_DIR) + 1) * sizeof(char));
    sprintf(dir, "%s%s", IMG_DIR, TEXTURE_DIR);
     
    TEXTURE_DIR = dir;
	if ((res = create_directory(TEXTURE_DIR)) == 0) {
		printf("'%s' created.\n", TEXTURE_DIR);
	} if (res == 1) fprintf(stderr, "'%s' already exists.\n", TEXTURE_DIR);
    dir = (char*)malloc(img_dir_len + strlen((SEPIA_DIR) + 1) * sizeof(char));
    sprintf(dir, "%s%s", IMG_DIR, (SEPIA_DIR));
     
    SEPIA_DIR = dir;
	if ((res = create_directory(SEPIA_DIR)) == 0) {
		printf("'%s' created.\n", SEPIA_DIR);
	}if (res == 1) fprintf(stderr, "'%s' already exists.\n", SEPIA_DIR);
    return (void*)0;
}

void* Check_Input_Args(int argc, char* argv[]) {
    //printf("argv[0]: %s\n", argv[0]); // Dbg purpose; to delete
    //printf("argv[1]: %s\n", argv[1]); // Dbg purpose; to delete
    //printf("argv[2]: %s\n", argv[2]); // Dbg purpose; to delete
    const char* help = "Wrong calling. Usage example with 4 threads: './old-photo-parallel-A ./dir-1 4 -size (or -name)'. Exiting.";
    if (argc < 4) {
        puts(help);
        exit(-1);
    } else {
        char *endptr;
        n_threads = strtol(argv[2], &endptr, 10); // Base 10
        if (*endptr != '\0') {
            fprintf(stderr, "Invalid number for threads: %s\n", argv[2]);
            puts(help);
            exit(-1);
        }
    }   
    //printf("n_threads: %d\n", n_threads); // Dbg purpose; to delete
    OPTION = argv[3];
    //printf("argv[3]: %s\n", OPTION);
    if (!n_threads) {
        puts("Invalid positive number.");
        puts(help);
        exit(-1);
    } else if ((strcmp(OPTION, "-size") != 0 ) && (strcmp(OPTION, "-name") != 0)) {
        puts("Invalid image ordering option.");
        puts(help);
        exit(-1);
    }
    IMG_DIR = argv[1];
    //strcat(IMG_DIR, "/");
    if (IMG_DIR[strlen(IMG_DIR) - 1] == '/') IMG_DIR[strlen(IMG_DIR) - 1] = '\0';
    printf("Number of threads will be: %d\n", n_threads); // Dbg purpose; to delete 
    printf("Image directory is: %s\n", IMG_DIR);          // Dbg purpose; to delete
    return (void*)0;
}

char** Read_Files_List() {
    files = (char*)malloc(1000 * sizeof(char*));
    bool isFileList = true;
    FILE* fp = 0;
    const int name_size = 100; // maximum size in chars of an image filename
    char img_name[name_size];
    char* img_file = 0;
    char* list;
    size_t img_dir_len = strlen(IMG_DIR);
    list = (char*)malloc((img_dir_len + strlen(IMG_LIST) + 1) * sizeof(char));
    sprintf(list, "%s/%s", IMG_DIR, IMG_LIST);
     
    IMG_LIST = list;
    printf("Reading '%s'.\n", IMG_LIST);
    fp = fopen(IMG_LIST, "rb");
    if (!fp) {
        fprintf(stderr, "File %s can't be opened or does not exist.\n", IMG_LIST);
        isFileList = false;
        puts("Checking for images in image folder.");
        if (!Check_for_Images()) {
            puts("No images found of formats .jpg, .jpeg or .png. Exiting.");
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
            fclose(fp);
            puts("Cannot process images from list. Aborting.");
            fp = 0;
            exit(-1);
        }
        int i = 0;
        while (fgets(img_name, name_size, fp) != 0) { //perhaps 'fscanf' is a better option: fscanf(fp, "%s", img_name); 
            img_file = strtok((char*)img_name, "\n");
            if (!img_file) continue;
            //printf("img_file: %s\n", img_file);  // Dbg purpose; to delete
            char* file_format = strstr(img_file, image_format);
            //printf("strstr: %s\n", file_format);  // Dbg purpose; to delete
            if (file_format != NULL && !strcmp(file_format, image_format)) {
                files[i] = (char*)malloc((strlen(img_name) + 1) * sizeof(char));
                if (!files[i]) {
                    printf("Cannot process %s. Skipping this one.\n", img_name);
                    continue;
                } 
                strcpy(files[i], img_file);
                //puts(files[i]); // Dbg purpose; to delete 
                i++;
            }
        }
    } else {
        DIR* dir;
        dir = opendir(IMG_DIR);
        if (!dir) {
            fprintf(stderr, "Directory %s does not exist. Exiting.\n", IMG_DIR);
            exit(-1);
        }
        struct dirent* entry;
        struct stat st;
        int i = 0;
        while ((entry = readdir(dir)) != NULL) {
            char* img_path;
            img_path = (char*)malloc((2 * strlen(IMG_DIR) + 1) * sizeof(char));
            sprintf(img_path, "%s/%s", IMG_DIR, entry->d_name);
            if (!stat(img_path, &st)) {
                if (S_ISREG(st.st_mode)) {
                    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
                    strncpy(img_name, entry->d_name, sizeof(img_name) - 1);
                    img_name[sizeof(img_name) - 1] = '\0';
                    img_name[sizeof(entry->d_name) - 1] = '\0';
                    if (Check_for_Extension(img_name, image_format)) {
                        //printf("Found image of format: %s\n", image_format);
                        //puts(img_name); // Dbg purpose; to delete
                        files[i] = (char*)malloc((strlen(img_name) + 1)* sizeof(char));
                        if (!files[i]) {
                            printf("Cannot process %s. Skipping this one.\n", files[i]);
                            free(img_path);
                            continue;
                        }
                        strcpy(files[i], img_name);
                        printf("Found image of %s, put file. Next.\n", files[i] );
                        i++;
                        n_img++;
                    }
                }
            }
            free(img_path);
        }
        if(dir) closedir(dir);
    }
    if (fp) {
        fclose(fp);
        fp = 0;
    }
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
    const int name_size = 100; // maximum size in chars of an image filename
    char img_name[name_size];
    DIR* dir;
 loop:  dir = opendir(IMG_DIR);
    if (!dir) {
        fprintf(stderr, "Directory %s does not exist. Exiting.\n", IMG_DIR);
        exit(-1);
    }
    struct dirent* entry;
    struct stat st;
    while ((entry = readdir(dir)) != NULL) {
        char* file_path;
        file_path = (char*)malloc((2 * strlen(IMG_DIR) + 1) * sizeof(char));
        sprintf(file_path, "%s/%s", IMG_DIR, entry->d_name);
        if (!stat(file_path/*entry->d_name*/, &st)) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
             if (S_ISREG(st.st_mode)) {
                strncpy(img_name, entry->d_name, sizeof(img_name));
                img_name[sizeof(img_name) - 1] = '\0';
                //printf("img_name: '%s'\n", img_name); // Dbg purpose; to delete
                if (Check_for_Extension(entry->d_name, file_format[n_formats])) {
                    res = true;
                    image_format = file_format[n_formats];
                    printf("Found image(s) of format: %s\n", image_format);
                    break;
                }
            }
        }
    }
    closedir(dir);
    if(!res && n_formats) {
        n_formats--;
        goto loop;
    }
    return res;
}

_Bool Check_for_Extension(char* filename, char* ext) {
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
    if (strcmp(OPTION, "-size") != 0) {
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
	clock_gettime(CLOCK_REALTIME, &start_time_total);
    clock_gettime(CLOCK_REALTIME, &start_time_ser);
    return (void*)0;
}

void* FinishTimingSerial() {
    clock_gettime(CLOCK_REALTIME, &end_time_ser); 
    struct timespec ser_time = diff_timespec(&end_time_ser, &start_time_ser);
    FILE *fp;
    char* timing = (char*)malloc(100 * sizeof(char));
    timing_file = (char*)malloc(100 * sizeof(char));
    sprintf(timing_file, "%s%s", IMG_DIR,"/timing_");
    timing_file = strcat(timing_file, "<");
    char* str_n_threads = (char*)malloc(3 * sizeof(char));
    sprintf(str_n_threads, "%d", n_threads);
    timing_file = strcat(timing_file, str_n_threads);
    sprintf(timing_file, "%s>%s.txt", timing_file, OPTION);
    fp = fopen(timing_file, "w");
    if (!fp) {
        fprintf(stderr, "File %s can't be created.\n", timing_file);
        return (void*) -1;
    }
    sprintf(timing, "\tser \t %10jd.%09ld\n", ser_time.tv_sec, ser_time.tv_nsec);
    if (fputs(timing, fp) == EOF) {
        fprintf(stderr, "Error on writing in file %s.\n", timing_file);
        fclose(fp);
        fp = 0;
        return (void*) -1;
    }
    fclose(fp);
    fp = 0;
    return (void*)0;
}

void* FinishTiming() {
    clock_gettime(CLOCK_REALTIME, &end_time_total);
    struct timespec total_time = diff_timespec(&end_time_total, &start_time_total);
    char* timing = (char*)malloc(100 * sizeof(char));
    FILE *fp;
    fp = fopen(timing_file, "a");
    if (!fp) {
        fprintf(stderr, "File %s can't be appended.\n", timing_file);
        return (void*) -1;
    }
    sprintf(timing, "\ttotal \t %10jd.%09ld\n", total_time.tv_sec, total_time.tv_nsec);
    if (fputs(timing, fp) == EOF) {
        fprintf(stderr, "Error on writing in file %s.\n", timing_file);
        fclose(fp);
        fp = 0;
        return (void*) -1;
    }
    fclose(fp);
    fp = 0;
    return (void*)0;
}

void* GetParallelTiming(struct timespec* start, struct timespec* end, long int thr_id) {
    struct timespec par_time = diff_timespec(end, start);
    char* timing = (char*)malloc(100 * sizeof(char));
    FILE *fp;
    fp = fopen(timing_file, "a");
    if (!fp) {
        fprintf(stderr, "File %s can't be appended.\n", timing_file);
        return (void*) -1;
    }
    sprintf(timing, "\tpar %ld\t %10jd.%09ld\n", thr_id, par_time.tv_sec, par_time.tv_nsec);
    if (fputs(timing, fp) == EOF) {
        fprintf(stderr, "Error on writing in file %s.\n", timing_file);
        fclose(fp);
        fp = 0;
        return (void*) -1;
    }
    fclose(fp);
    fp = 0;
    return (void*)0;
}