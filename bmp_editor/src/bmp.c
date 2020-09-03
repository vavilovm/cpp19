#include "bmp.h"

int padd(int width) {
	return (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
}

void free_image(IMAGE *image) {
	free(image->data);
}


IMAGE *load_bmp(char *infile, IMAGE *image) {
	assert(image);
	//assert fileheaders size
	assert(sizeof(BITMAPFILEHEADER) == 14 && sizeof(BITMAPINFOHEADER) == 40);

	// open file
	FILE *filePtr = fopen(infile, "rb");
	
	//check for null
	if (filePtr == NULL) {
		return NULL;
	}

	// read infile's BITMAPFILEHEADER
	if (fread(&image->bf, sizeof(BITMAPFILEHEADER), 1, filePtr) != 1) {
		fclose(filePtr);
		return NULL;
	}
	

	// read infile's BITMAPINFOHEADER
	if (fread(&image->bi, sizeof(BITMAPINFOHEADER), 1, filePtr) != 1) {
		fclose(filePtr);
		return NULL;
	}

	//move file point to the begging of bitmap data
	fseek(filePtr, image->bf.bfOffBits, SEEK_SET);

	// allocate enough memory for the bitmap IMAGE data
	RGBTRIPLE *bitmapImage;
	bitmapImage = (RGBTRIPLE*)malloc(image->bi.biSizeImage);

	// verify memory allocation
	if (!bitmapImage)
	{
		fclose(filePtr);
		return NULL;
	}


	int height = image->bi.biHeight;
	int width = image->bi.biWidth;
	int inPadding = padd(width);


	// read in the bitmap IMAGE data
	for (int i = 0; i < abs(height); i++) {

		// store line without padding to pixel array 
		if(fread(bitmapImage + i * width, width * sizeof(RGBTRIPLE), 1, filePtr) != 1) {
			free(bitmapImage);
			fclose(filePtr);
			return NULL;
		}	
		fseek(filePtr, inPadding, SEEK_CUR); //skip padding 
	}


	// remember the pixel array 
	image->data = bitmapImage;

	//success
	fclose(filePtr);
	return image;
}

// image is modified
int crop (IMAGE *image, LONG x, LONG y, LONG w, LONG h) {

	// check input for correctness
	if (!(x + w <= image->bi.biWidth && y + h <= abs(image->bi.biHeight))) {
		return 1;
	}

 	// fix y
	y = abs(image->bi.biHeight) - (h + y); // new y is offset below: image stores data from from below
	
	LONG oldw = image->bi.biWidth;

	// update info
	image->bi.biWidth = w;
	image->bi.biHeight = (image->bi.biHeight < 0) ?  (-h) : h;

	int outPadding = padd(image->bi.biWidth);


	image->bi.biSizeImage = (sizeof(RGBTRIPLE) * image->bi.biWidth + outPadding) * abs(image->bi.biHeight);
	image->bf.bfSize = image->bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	

	RGBTRIPLE *bitmapImage;
	bitmapImage = (RGBTRIPLE*)malloc(image->bi.biSizeImage);
	if(!bitmapImage) {
		return 2;
	}

	//current pos in new image
	RGBTRIPLE *pos = bitmapImage;

	// current pos in old image
	RGBTRIPLE *cur = image->data + x + y * image->bi.biWidth;

	for (LONG i = 0; i < h; i++) {
		memcpy(pos, cur, w * sizeof(RGBTRIPLE));
		pos += w;
		cur += oldw;
	}
	
	free(image->data);	
	
	image->data = bitmapImage;
		
	// success
	return 0;
}


IMAGE *rotate (IMAGE *image) {
	RGBTRIPLE *newImage;
	newImage = (RGBTRIPLE*)malloc(image->bi.biSizeImage);
	if (!newImage) {
		return NULL;
	}
	RGBTRIPLE *oldImage = image->data;

	size_t oldh = image->bi.biHeight; 
	size_t oldw = image->bi.biWidth;	
	size_t h = image->bi.biWidth; //new height
	size_t w = image->bi.biHeight; //new width

	for (int i = 0; i < abs(h); i++) {
		for (size_t j = 0; j < w; j++){
			newImage[i * w + j] = oldImage[j * oldw + h - 1 - i];
		}
	}

	image->data = newImage;
	image->bi.biWidth = oldh;
	image->bi.biHeight = oldw;

	int outPadding = padd(image->bi.biWidth);

	image->bi.biSizeImage = (sizeof(RGBTRIPLE) * image->bi.biWidth + outPadding) * abs(image->bi.biHeight);
	image->bf.bfSize = image->bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	free(oldImage);

	return image;
}


IMAGE *save_bmp(IMAGE *image, char *outfile) {
	// open file
	FILE *fout = fopen(outfile, "wb");
	
	assert(image);

	//check for null
	if (fout == NULL) {
		return NULL;
	}

	int newHight = image->bi.biHeight;
	int newWidth = image->bi.biWidth;
    int outPadding = padd(newWidth);


	// write outfile's BITMAPFILEHEADER
    if (fwrite(&image->bf, sizeof(BITMAPFILEHEADER), 1, fout) != 1) {
    	return NULL;
    }

    // write outfile's BITMAPINFOHEADER
    if (fwrite(&image->bi, sizeof(BITMAPINFOHEADER), 1, fout) != 1) {
    	return NULL;
    }

    // write IMAGE
    for (int j = 0; j < abs(newHight); j++) {
            // write a new scanline once
            fwrite(image->data + j * newWidth, sizeof(RGBTRIPLE) * newWidth, 1, fout);

            // write new padding
            for (int k = 0; k < outPadding; k++)
            {
                fputc(0x00, fout);
            }
    }
    
    free_image(image);
    fclose(fout);
    return image;
}
