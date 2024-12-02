 #include "Processa_ficheiros.h"
 
const int name_size = 100;
extern int n_img;

int Processa_smooth(char** ffiles, int n_files, int index) { 
	/* file name of the image created and to be saved on disk	 */
	char out_file_name[name_size];
	/* input images */
	gdImagePtr in_img;
	/* output images */
	gdImagePtr out_smooth_img;
	/* Iteration over all the files to create thumbnails from images*/
	char* img_from_dir = IMG_DIR; // Base directory of image
	strcat(img_from_dir, "/");
	for (int i = index; i < n_files; i++) {	
		strcat(img_from_dir, files[i]); //Path to image
		int file_exists = Check_existing_image(img_from_dir);
		if (file_exists) continue;
		printf("smooth %s\n", files[i]);
		/* load of the input file */
		if (image_format == jpg_file || image_format == jpeg_file) {
			in_img = read_jpeg_file(img_from_dir);
		} else {
			in_img = read_png_file(img_from_dir);
		}
		if (!in_img){
			fprintf(stderr, "Impossible to read %s image\n", files[i]);
			continue;
		}
		/* creation of thumbnail from image */
		out_smooth_img = smooth_img(in_img);
  		if (!out_smooth_img) fprintf(stderr, "Impossible to create smooth of %s image\n", files[i]);
        else {
			/* save smooth image */
			sprintf(out_file_name, "%s%s", SMOOTH_DIR, files[i]);
			if (image_format == jpg_file || image_format == jpeg_file) {
				if(!write_jpeg_file(out_smooth_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
			} else {
				if(!write_png_file(out_smooth_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
			}
			
		}
		gdImageDestroy(out_smooth_img);
		gdImageDestroy(in_img);
	}
	exit(0);
}

int Processa_texture(char** ffiles, int n_files, int index) {

 	puts("In Processa_texture...");
 	//puts("files here:"); 
	//for (int i = 0; i < n_img; i++) puts(files[i]); // Dbg purpose; to delete

	/* input images */
	gdImagePtr in_img,  texture_img;
	/* output image */
	gdImagePtr out_texture_img; 
	/* file name of the image created and to be saved on disk */
	char out_file_name[100];
    texture_img = read_png_file("paper-texture.png");
	if(!texture_img) {
		puts("No texture...");
		fprintf(stderr, "Impossible to read %s image\n", "paper-texture.png");
		exit(-1);
	} else puts("Texturing...");
	sprintf(out_file_name, "%s%s", TEXTURE_DIR, "paper-texture.png");
	if (!write_png_file(texture_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
    else {
	//char* img_from_dir = malloc(name_size * sizeof(char));
	//sprintf(img_from_dir, "%s%c", IMG_DIR, '/');
	//strcpy(img_from_dir, IMG_DIR); // Base directory of image
	//strcat(img_from_dir, "/");
	//puts(img_from_dir);
	/* Iteration over all the files to watermark images*/
    	//printf("In ..._texture; index: %d,  n_files: %d\n", index, n_files); // Dbg purpose; to delete
		//if (!files) puts("!!NO FILES!!");
		char* img_from_dir = IMG_DIR; // Base directory of image
		strcat(img_from_dir, "/");
		for (int i = index; i < n_files; i++) {	
		//puts("Reading file");
		//puts(files[i]);
	    	strcat(img_from_dir, files[i]); //Path to image
		//sprintf(img_from_dir, "%s%s", img_from_dir, files[i]);

//sprintf(img_from_dir, "%s%s", img_from_dir, files[i]);

		//puts(img_from_dir);
		
			int file_exists = Check_existing_image(img_from_dir);
			if (file_exists) continue;
	    	/* load of the input file */
			if (image_format == jpg_file || image_format == jpeg_file) {
				in_img = read_jpeg_file(img_from_dir);
			} else {
				in_img = read_png_file(img_from_dir);
			}
			printf("Texture  %s\n", files[i]);
			if (in_img == NULL)  {
				printf("Impossible to read %s image\n", img_from_dir);
				continue;
			} else printf("Read %s image\n", img_from_dir);
		/* add watermark */
			out_texture_img = texture_image(in_img, texture_img);
  			if (!out_texture_img) {
            	fprintf(stderr, "Impossible to create smooth of %s image\n", files[i]);
       		} else {
			/* save texture */
				sprintf(out_file_name, "%s%s", TEXTURE_DIR, files[i]);
				if (image_format == jpg_file || image_format == jpeg_file) {
					if(!write_jpeg_file(out_texture_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
				} else {
					if(!write_png_file(out_texture_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
				}
			gdImageDestroy(out_texture_img);
			gdImageDestroy(in_img);
		//sprintf(img_from_dir, "%s%c", IMG_DIR, '/');
		//strcpy(img_from_dir, IMG_DIR);
		//strcat(img_from_dir, "/");
		//puts("New img_from_dir");
		//puts(img_from_dir);
		}
		gdImageDestroy(texture_img);	
	}
	exit(0);
}

int Processa_contrast(char** ffiles, int n_files, int index) {
	/* file name of the image created and to be saved on disk	 */
	char out_file_name[name_size];
	/* input images */
	gdImagePtr in_img;
	/* output images */
	gdImagePtr out_contrast_img;
	char* img_from_dir = IMG_DIR; // Base directory of image
	strcat(img_from_dir, "/");
	/* Iteration over all the files to resize images */
	for (int i = index; i < n_files; i++) {	
		strcat(img_from_dir, files[i]); //Path to image
		int file_exists = Check_existing_image(img_file);
		if (file_exists) continue;
		/* load of the input file */
	    printf("contrast %s\n", files[i]);
		if (image_format == jpg_file || image_format == jpeg_file) {
			in_img = read_jpeg_file(img_from_dir);
		} else {
			in_img = read_png_file(img_from_dir);
		}
		if (!in_img){
			printf("Impossible to read %s image\n", img_from_dir);
			continue;
		}
		out_contrast_img = contrast_image(in_img);
  		if (!out_contrast_img) fprintf(stderr, "Impossible to create contrast of %s image\n", files[i]);
        else {
			/* save contrast image*/
			sprintf(out_file_name, "%s%s", CONTRAST_DIR, files[i]);
			if (image_format == jpg_file || image_format == jpeg_file) {
				if(!write_jpeg_file(out_contrast_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
			} else
				if(!write_png_file(out_contrast_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
		}
		gdImageDestroy(out_contrast_img);
		gdImageDestroy(in_img);
	}
	exit(0);
}

int Processa_sepia(char** ffiles, int n_files, int index) { 
	/* file name of the image created and to be saved on disk	 */
	char out_file_name[name_size];
	/* input images */
	gdImagePtr in_img;
	/* output images */
	gdImagePtr out_sepia_img;
	/* Iteration over all the files to create thumbnails from images*/
	char* img_from_dir = IMG_DIR; // Base directory of image
	strcat(img_from_dir, "/");
	for (int i = index; i < n_files; i++) {
		strcat(img_from_dir, files[i]); //Path to image
		int file_exists = Check_existing_image(img_file);
		if (file_exists) continue;	
	   	printf("sepia %s\n", files[i]);
		/* load of the input file */
		if (image_format == jpg_file || image_format == jpeg_file) {
			in_img = read_jpeg_file(img_from_dir);
		} else {
			in_img = read_png_file(img_from_dir);
		}
		if (!in_img){
			fprintf(stderr, "Impossible to read %s image\n", files[i]);
			continue;
		}
		/* creation of sepia from image */
		out_sepia_img = sepia_image(in_img);
  		if (!out_sepia_img) fprintf(stderr, "Impossible to create sepia of %s image\n", files[i]);
        else {
			/* save sepia image */
			sprintf(out_file_name, "%s%s", SEPIA_DIR, files[i]);
			if (image_format == jpg_file || image_format == jpeg_file) {
				if(!write_jpeg_file(out_sepia_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
			} else
			if(!write_png_file(out_sepia_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
		}
		gdImageDestroy(out_sepia_img);
		gdImageDestroy(in_img);
	}
	exit(0);
}

int Check_existing_image(char* img_file) {
	int res = 0;
	if (access(img_file, F_OK) != -1) {
		printf("%s encontrado\n", img_file);
		res = 1;
	} else printf("%s nao encontrado\n", img_file);
	return res;
}