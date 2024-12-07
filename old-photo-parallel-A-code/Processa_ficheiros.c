#include "Processa_ficheiros.h"
#include <stdbool.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const int name_size = 100;
extern int n_img;
extern char* image_format; 
extern const char*  jpeg_file, jpg_file;

int Processa_smooth(int n_files, int index, char** files) { 
	/* file name of the image created and to be saved on disk	 */
	/* input images */
	gdImagePtr in_img;
	/* output images */
	gdImagePtr out_smooth_img;
	/* Iteration over all the files to create thumbnails from images*);*/
	//puts("files in Processa_smooth:"); for (int i = 0; i < n_img; i++) puts(files[i]); // Dbg purpose; to delete

	for (int i = index; i < index + n_files; i++) {	
		char* out_file_name = malloc(name_size * sizeof(char));
		sprintf(out_file_name, "%s%s", TEXTURE_DIR, files[i]);
		char* img_from_dir= malloc(name_size * sizeof(char));; // Base directory of image
		sprintf(img_from_dir, "%s%s", SMOOTH_DIR, files[i]);
		int file_exists = Check_existing_image(out_file_name);
		if (file_exists) continue;
		printf("Smooth %s\n", files[i]);
		/* load of the input file */
		//if (strcmp((const char*)image_format, (const char*)jpg_file) == 0 || strcmp((const char*)image_format, (const char*)jpeg_file) == 0) {
			in_img = read_jpeg_file(img_from_dir);
		//} else {
		//	in_img = read_png_file(img_from_dir);
		//}
		if (!in_img){
			fprintf(stderr, "Impossible to read %s image\n", files[i]);
			continue;
		}
		/* creation of thumbnail from image */
		out_smooth_img = smooth_image(in_img);
  		if (!out_smooth_img) fprintf(stderr, "Impossible to create smooth of %s image\n", files[i]);
        else {
			/* save smooth image */
			//if (strcmp((const char*)image_format, (const char*)jpg_file) == 0 || strcmp((const char*)image_format, (const char*)jpeg_file) == 0) {
				if(!write_jpeg_file(out_smooth_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
			//} else {
			//	if(!write_png_file(out_smooth_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
			//}
			
		}
		gdImageDestroy(out_smooth_img);
		gdImageDestroy(in_img);
		free(out_file_name);
		free(img_from_dir);
	}
	return 0;
}

int Processa_texture(int n_files, int index, char** files) {
 	//puts("In Processa_texture...");
	/* input images */
	gdImagePtr in_img,  texture_img;
	/* output image */
	gdImagePtr out_texture_img; 
	/* file name of the image created and to be saved on disk */
	bool texture_exists = true;
	puts("files in Processa_texture:"); for (int i = 0; i < n_img; i++) puts(files[i]); // Dbg purpose; to delete
	printf("n_files: %d, index: %d.\n", n_files, index);
	
    texture_img = read_png_file("./old-photo-parallel-A-code/paper-texture.png");
	if(!texture_img) {
		puts("No texture...");
		fprintf(stderr, "Impossible to read %s image. Skipping texturing.\n", "paper-texture.png");
		texture_exists = false;
	} else { puts("Texturing...");
		char* out_file_name = malloc(name_size * sizeof(char));
		sprintf(out_file_name, "%s%s", SEPIA_DIR, "paper-texture.png");
		int file_exists = Check_existing_image(out_file_name);
		if (!file_exists) {
			if (!write_png_file(texture_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
    		texture_exists = false;
		}
		free(out_file_name);
	}
	if (texture_exists) {
    	for (int i = index; i < index + n_files; i++) {	
	    	char* out_file_name = malloc(name_size * sizeof(char));
			sprintf(out_file_name, "%s%s", SEPIA_DIR, files[i]);
	    	printf("out_file_name: '%s'.\n", out_file_name);
			char* img_from_dir= malloc(name_size * sizeof(char));; // Base directory of image
			sprintf(img_from_dir, "%s%s", TEXTURE_DIR, files[i]);
			printf("img_from_dir: '%s'.\n", img_from_dir);int file_exists = Check_existing_image(out_file_name);
			if (file_exists) continue;
		/* load of the input file */
			//if (strcmp((const char*)image_format, (const char*)jpg_file) == 0 || strcmp((const char*)image_format, (const char*)jpeg_file) == 0) {
				in_img = read_jpeg_file(img_from_dir);
			//} else {
			//	in_img = read_png_file(img_from_dir);
			//}
			printf("Texture  %s\n", files[i]);
			if (in_img == NULL)  {
				printf("Impossible to read %s image\n", img_from_dir);
				continue;
			} else printf("Read %s image\n", img_from_dir);
		/* add texture */
			out_texture_img = texture_image(in_img, texture_img);
  			if (!out_texture_img) {
            	fprintf(stderr, "Impossible to create texture of %s image\n", files[i]);
       		} else {
			/* save texture */
				//if (strcmp((const char*)image_format, (const char*)jpg_file) == 0 || strcmp((const char*)image_format, (const char*)jpeg_file) == 0) {
					if(!write_jpeg_file(out_texture_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
				//} else {
				//	if(!write_png_file(out_texture_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
				//}
				gdImageDestroy(out_texture_img);
				gdImageDestroy(in_img);
				free(out_file_name);
				free(img_from_dir);
			}	
		}
	} else {
		int smooth_fd = open(TEXTURE_DIR, O_RDONLY);
		int sepia_fd = open(SEPIA_DIR, O_WRONLY);
		struct stat stat_buf;
    	fstat(smooth_fd, &stat_buf);
		off_t offset = 0;
		sendfile(sepia_fd, smooth_fd, &offset, stat_buf.st_size	);
		close(smooth_fd);
        close(sepia_fd);
	}
	gdImageDestroy(texture_img);	
	return 0;
}

int Processa_contrast(int n_files, int index, char** files) {	
	/* input images */
	gdImagePtr in_img;
	/* output images */
	gdImagePtr out_contrast_img;
	/* Iteration over all the files to contrast images */
    //puts("files in Processa_contrast:"); for (int i = 0; i < n_img; i++) puts(files[i]); // Dbg purpose; to delete

	//printf("n_files: %d, index: %d.\n", n_files, index);
	for (int i = index; i < index + n_files; i++) {
		/* file name of the image created and to be saved on disk	 */
		char* out_file_name = malloc(name_size * sizeof(char));
		char* img_from_dir= malloc(name_size * sizeof(char));; // Base directory of image
		sprintf(out_file_name, "%s%s", SMOOTH_DIR, files[i]);
		//printf("out_file_name: '%s'\n", out_file_name);
		sprintf(img_from_dir, "%s/%s", IMG_DIR, files[i]);
		//printf("img_from_dir: '%s'.\n", img_from_dir);
		int file_exists = Check_existing_image(out_file_name);
		if (file_exists) continue;
		printf("Contrast %s\n", files[i]);
		
		//if (strcmp((const char*)image_format, (const char*)jpg_file) == 0 || strcmp((const char*)image_format, (const char*)jpeg_file) == 0) {
			in_img = read_jpeg_file(img_from_dir);
		//} else {
		//	in_img = read_png_file(img_from_dir);
		//}
		if (!in_img){
			printf("Impossible to read %s image\n", img_from_dir);
			continue;
		}
		out_contrast_img = contrast_image(in_img);
  		if (!out_contrast_img) fprintf(stderr, "Impossible to create contrast of %s image\n", files[i]);
         else {
			/* save contrast image*/
			//if (strcmp((const char*)image_format, (const char*)jpg_file) == 0 || strcmp((const char*)image_format, (const char*)jpeg_file) == 0) {
				if(!write_jpeg_file(out_contrast_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
			//} else
			//	if(!write_png_file(out_contrast_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
		}
		gdImageDestroy(out_contrast_img);
		gdImageDestroy(in_img);
		free(out_file_name);
		free(img_from_dir);
	}
	return 0;
}

int Processa_sepia(int n_files, int index, char** files) { 
	/* input images */
	gdImagePtr in_img;
	/* output images */
	gdImagePtr out_sepia_img;
	/* Iteration over all the files to create thumbnails from images*/
    puts("files in Processa_sepia:"); for (int i = 0; i < n_img; i++) puts(files[i]); // Dbg purpose; to delete

	for (int i = index; i < index + n_files; i++) {
		/* file name of the image created and to be saved on disk	 */
		char* out_file_name = malloc(name_size * sizeof(char));
		char* img_from_dir= malloc(name_size * sizeof(char));; // Base directory of image
		sprintf(img_from_dir, "%s%s", SEPIA_DIR, files[i]);
		sprintf(out_file_name, "%s%s%s", IMG_DIR, OLD_PHOTO_PAR_A, files[i]);
		int file_exists = Check_existing_image(out_file_name);
		if (file_exists) continue;
	   	printf("Sepia %s\n", files[i]);
		/* load of the input file */
		//if (strcmp((const char*)image_format, (const char*)jpg_file) == 0 || strcmp((const char*)image_format, (const char*)jpeg_file) == 0) {
			in_img = read_jpeg_file(img_from_dir);
		//} else {
			//in_img = read_png_file(img_from_dir);
		//}
		if (!in_img){
			fprintf(stderr, "Impossible to read %s image\n", files[i]);
			continue;
		}
		/* creation of sepia from image */
		out_sepia_img = sepia_image(in_img);
  		if (!out_sepia_img) fprintf(stderr, "Impossible to create sepia of %s image\n", files[i]);
        else {
			/* save sepia image */
			//if (strcmp((const char*)image_format, (const char*)jpg_file) == 0 || strcmp((const char*)image_format, (const char*)jpeg_file) == 0) {
				if(!write_jpeg_file(out_sepia_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
			//} else
			//if(!write_png_file(out_sepia_img, out_file_name)) fprintf(stderr, "Impossible to write %s image\n", out_file_name);
		}
		gdImageDestroy(out_sepia_img);
		gdImageDestroy(in_img);
		free(out_file_name);
		free(img_from_dir);
	}
	return 0;
}

int Check_existing_image(char* img_file) {
	int res = 0;
	if (access(img_file, F_OK) != -1) {
		printf("%s encontrado\n", img_file);
		res = 1;
	} else printf("%s nao encontrado\n", img_file);
	return res;
}