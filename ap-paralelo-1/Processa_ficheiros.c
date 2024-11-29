 #include "Processa_ficheiros.h"
 
const int name_size = 100;
extern int n_img;
int Processa_watermarks(char** ffiles, int n_files, int index) {


	puts("In Processa_watermarks...");
 puts("files here:"); for (int i = 0; i < n_img; i++) puts(files[i]); // Dbg purpose; to delete

	/* input images */
	gdImagePtr in_img,  watermark_img;
	/* output image */
	gdImagePtr out_watermark_img; 
	/* file name of the image created and to be saved on disk */
	char out_file_name[100];
    watermark_img = read_png_file("watermark.png");
	if(!watermark_img){
		puts("No Watermark...");
		fprintf(stderr, "Impossible to read %s image\n", "watermark.png");
		exit(-1);
	} else puts("Watermarking...");


sprintf(out_file_name, "%s%s", WATER_DIR, "watermark.png");
if(!write_png_file(watermark_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);



	char* img_from_dir = malloc(name_size * sizeof(char));
	//sprintf(img_from_dir, "%s%c", IMG_DIR, '/');
	strcpy(img_from_dir, IMG_DIR); // Base directory of image
	strcat(img_from_dir, "/");
	puts(img_from_dir);
	/* Iteration over all the files to watermark images*/
    printf("In ..._watermarks; index: %d,  n_files: %d\n", index, n_files); // Dbg purpose; to delete
	if (!files) puts("!!NO FILES!!");
	for (int i = index; i < n_files; i++) {	
		puts("Reading file");
		puts(files[i]);
	    printf("Watermarking  %s\n", files[i]);
		strcat(img_from_dir, files[i]); //Path to image
		//sprintf(img_from_dir, "%s%s", img_from_dir, files[i]);

//sprintf(img_from_dir, "%s%s", img_from_dir, files[i]);

		puts(img_from_dir);
		/* load of the input file */
	    in_img = read_png_file(img_from_dir);
		if (in_img == NULL)  {
			printf("Impossible to read %s image\n", img_from_dir);
			continue;
		} else printf("Read %s image\n", img_from_dir);
		/* add watermark */
		out_watermark_img = add_watermark(in_img, watermark_img);
  		if (!out_watermark_img) {
            fprintf(stderr, "Impossible to create thumbnail of %s image\n", files[i]);
        }else{
			/* save watermark */
			sprintf(out_file_name, "%s%s", WATER_DIR, files[i]);
			if(!write_png_file(out_watermark_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
			gdImageDestroy(out_watermark_img);
		}
		gdImageDestroy(in_img);
		//sprintf(img_from_dir, "%s%c", IMG_DIR, '/');
		strcpy(img_from_dir, IMG_DIR);
		strcat(img_from_dir, "/");
		puts("New img_from_dir");
		puts(img_from_dir);
	}	
	gdImageDestroy(watermark_img);
	exit(0);
}

int Processa_resizes(char** ffiles, int n_files, int index) {
	/* file name of the image created and to be saved on disk	 */
	char out_file_name[name_size];
	/* input images */
	gdImagePtr in_img;
	/* output images */
	gdImagePtr out_resized_img;
	char* img_from_dir = IMG_DIR; // Base directory of image
	strcat(img_from_dir, "/");
	/* Iteration over all the files to resize images */
	for (int i = index; i < n_files; i++) {	
		printf("resize %s\n", files[i]);
		strcat(img_from_dir, files[i]); //Path to image
		/* load of the input file */
	    in_img = read_png_file(img_from_dir);
		if (!in_img){
			printf("Impossible to read %s image\n", img_from_dir);
			continue;
		}
		out_resized_img = resize_image(in_img, 800);
  		if (!out_resized_img) fprintf(stderr, "Impossible to resize %s image\n", files[i]);
        else {
			/* save resized image*/
			sprintf(out_file_name, "%s%s", RESIZE_DIR, files[i]);
			if(!write_png_file(out_resized_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
			gdImageDestroy(out_resized_img);
		}
		gdImageDestroy(in_img);
	}
	exit(0);
}

int Processa_thumbnails(char** ffiles, int n_files, int index) { 
	/* file name of the image created and to be saved on disk	 */
	char out_file_name[name_size];
	/* input images */
	gdImagePtr in_img;
	/* output images */
	gdImagePtr out_thumb_img;
	/* Iteration over all the files to create thumbnails from images*/
	char* img_from_dir = IMG_DIR; // Base directory of image
	strcat(img_from_dir, "/");
	for (int i = index; i < n_files; i++) {	
	   	printf("thumbnail %s\n", files[i]);
		/* load of the input file */
		in_img = read_png_file(img_from_dir);
	    in_img = read_png_file(files[i]);
		if (!in_img){
			fprintf(stderr, "Impossible to read %s image\n", files[i]);
			continue;
		}
		/* creation of thumbnail from image */
		out_thumb_img = make_thumb(in_img, 150);
  		if (!out_thumb_img) fprintf(stderr, "Impossible to creat thumbnail of %s image\n", files[i]);
        else {
			/* save thumbnail image */
			sprintf(out_file_name, "%s%s", THUMB_DIR, files[i]);
			if(!write_png_file(out_thumb_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
		}
		gdImageDestroy(out_thumb_img);
		gdImageDestroy(in_img);
	}
	exit(0);
}

int Check_existing_images(char* img_file) {
	int res = 0;
	if (access(img_file, F_OK) != -1) {
		printf("%s encontrado\n", img_file);
		res = 1;
	} else printf("%s nao encontrado\n", img_file);
	return res;
}