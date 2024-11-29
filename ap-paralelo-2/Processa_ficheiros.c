 #include "Processa_ficheiros.h"
 
const int name_size = 100;

int Processa_watermarks(char* img_dir) {
	/* input images */
	gdImagePtr in_img,  watermark_img;
	/* output image */
	gdImagePtr out_watermark_img; 
	/* file name of the image created and to be saved on disk */
	char out_file_name[name_size];
	//char out_file_name[100];
    watermark_img = read_png_file("watermark.png");
	if(!watermark_img){
		fprintf(stderr, "Impossible to read %s image\n", "watermark.png");
		exit(-1);
	}
	char* img_from_dir = img_dir; // Base directory of image
	strcat(img_from_dir, "/");
	/* Iteration over all the files to watermark images*/
	for (int i = 0; i < n_files; i++) {		
	    printf("Watermarking  %s\n", files[i]);
		strcat(img_from_dir, files[i]); //Path to image
		/* load of the input file */
	    in_img = read_png_file(img_from_dir);
		if (!img_from_dir) fprintf(stderr, "Impossible to read %s image\n", files[i]);
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
	}	
	gdImageDestroy(watermark_img);
	exit(0);
}

int Processa_resizes(char* img_dir) {
	/* file name of the image created and to be saved on disk	 */
	char out_file_name[name_size];
	/* input images */
	gdImagePtr in_img;
	/* output images */
	gdImagePtr out_resized_img;
	char* img_from_dir = IMG_DIR; // Base directory of image
	strcat(img_from_dir, "/");
	/* Iteration over all the files to resize images */
	for (int i = 0; i < n_files; i++) {	
		printf("resize %s\n", files[i]);
		strcat(img_from_dir, files[i]); //Path to image
		/* load of the input file */
	    in_img = read_png_file(img_from_dir);
		if (!in_img){
			fprintf(stderr, "Impossible to read %s image\n", files[i]);
			continue;
		}
		/* resizes image */
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

int Processa_thumbnails(char* img_dir) { 
	/* file name of the image created and to be saved on disk	 */
	char out_file_name[name_size];
	/* input images */
	gdImagePtr in_img;
	/* output images */
	gdImagePtr out_thumb_img;
	/* Iteration over all the files to create thumbnails from images*/
	char* img_from_dir = IMG_DIR; // Base directory of image
	strcat(img_from_dir, "/");
	for (int i = 0; i < n_files; i++) {	
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

void* Processa_Watermarks(void* args) {
	char* img_dir = (char*) args;
	Processa_watermarks(img_dir);
	return (void*)0;
}

void* Processa_Resizes(void* args) {
	char* img_dir = (char*) args;
 	Processa_resizes(img_dir);
	return (void*)0;
}

void* Processa_Thumbnails(void* args) {
	char* img_dir = (char*) args;
	Processa_thumbnails(img_dir);
	return (void*)0;
}

int Check_existing_images(char* img_file) {
	int res = 0;
		int res = 0;
	if (access(img_file, F_OK) != -1) {
		printf("%s encontrado\n", img_file);
		res = 1;
	} else printf("%s nao encontrado\n", img_file);
	return res;
}