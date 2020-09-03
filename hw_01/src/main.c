#include "bmp.h"
#include "stego.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	// ensure proper usage
	if (argc < 2) {
		fprintf(stderr, "try insert or extract or crop-rotate\n");
		return 1;
	}

	// what to do
	char *action = argv[1];

	// ensure proper usage
	if (!strcmp(action, "crop-rotate")) {
		if (argc != 8) {
			fprintf(stderr, "Usage: crop-rotate ‹in-bmp› ‹out-bmp› ‹x› ‹y› ‹w› ‹h›\n");
			return -2;
		}
	} else if (!strcmp(action, "insert")) {		
		if (argc != 6) {
			fprintf(stderr,"Usage: insert ‹in-bmp› ‹out-bmp› ‹key-txt› ‹msg-txt›\n");
			return -3;
		}	
	} else if (!strcmp(action, "extract")) {
		if (argc != 5) {
			fprintf(stderr,	"Usage: extract ‹in-bmp› ‹key-txt› ‹msg-txt›\n");
			return -4;
		}
	}

	// store filenames
	char *infile = argv[2];
	char *outfile = argv[3];


	// load input image
	IMAGE image;
	if (!(load_bmp(infile, &image))) {
		fprintf(stderr, "Could not load %s.\n", infile);
		return 2;
	}

	// crop and rotate
	if (!strcmp(action, "crop-rotate")) {

		// remember where to start
		LONG x = atoi(argv[4]); //upper left corner 
		LONG y = atoi(argv[5]);

		// remember output size
		LONG newWidth = atoi(argv[6]);
		LONG newHeight = atoi(argv[7]);

		// dimensions are swapped to get asked image after rotate
		int err = crop(&image, x, y, newWidth, newHeight);
		if(err == 1) {
			free_image(&image);
			fprintf(stderr, "Could not crop, check input\n");
			return 3;
		} else if(err == 2) {
			free_image(&image);
			fprintf(stderr, "Memory allocation failed\n");
			return 3;
		}

		// rotate
		if(!rotate(&image)) {
			free_image(&image);
			fprintf(stderr, "Could not rotate\n");
			return 4;
		}
	} else if(!strcmp(action, "insert")) {			

			// get filenames
			char *key = argv[4];
			char *msg = argv[5];
			
			// call insert function
			if(!insert(&image, key, msg)) {
				free_image(&image);
				fprintf(stderr, "Could not insert\n");
				return 5;
			}

		// extract
	} else if (!strcmp(action, "extract")) {

		// get filenames
		char *key = argv[3];
		char *msg = argv[4];
		// call extract function
		if(!extract(&image, key, msg)) {
			free_image(&image);
			fprintf(stderr, "Could not insert\n");
			return 6;
		}
	} else {

		// some wrong input
		fprintf(stderr,	"try insert or extract or crop-rotate\n");
		return -1;
	}

	


	// save output bmp file if not extract
	if(strcmp(action, "extract") && save_bmp(&image, outfile) == NULL) {
		fprintf(stderr, "Could not create %s.\n", outfile);
		return 7;
	}


	// success
	return 0;
}