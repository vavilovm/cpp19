#include "stego.h"

char char_by_byte(BYTE byte) {
	if (byte == 26)
		return ' ';
	else if (byte == 27)
		return '.';
	else if (byte == 28)
		return ',';
	else return 'A' + byte;
}

BYTE byte_by_char(char chr) {
	if (chr == ' ')
		return 26;
	else if (chr == '.')
		return 27; 
	else if (chr == ',')
		return 28;
	else return chr - 'A';
}


BYTE *get_byte_by_color(RGBTRIPLE *pixel, char color) {  // x- pixel
	assert(pixel);
	
	// get the byte asked (by color)
	if (color == 'B') 
		return &pixel->rgbtBlue;
	else if (color == 'G')
		return &pixel->rgbtGreen;
	else if (color == 'R')
		return &pixel->rgbtRed;
	else return NULL;
}

// change last bit to "bit"
void change_last_bit(BYTE *byte, BYTE bit) {
	*byte = (*byte & 254) + bit; // 254  == 0x11111110
}


// insert message from file to image
char *insert (IMAGE *image, char *key_fn, char *msg_fn) {
	// open files to read
	FILE *fileKey = fopen(key_fn, "rb");
	FILE *fileMsg = fopen(msg_fn, "rb");
	
	//check for null
	if (fileKey == NULL || fileMsg == NULL) {
		fclose(fileKey);
		fclose(fileMsg);
		return NULL;
	}
	
	// pixel position 
	int x, y;

	// chr - char to insert
	char chr, color; 
	
	// BYTE encoded char
	BYTE byte; 

	// read file with a message char by char
	while ((chr = fgetc(fileMsg)) != EOF) {

		// init bitmask
		BYTE mask = 1 << 4; // filling left to right

		// get byte to encode
		byte = byte_by_char(chr);

		// store every bit according to keys
		for (int j = 4; j >= 0; j--) {
			
			// get the needed pixel from the keys file
			if (fscanf(fileKey, "%d %d %c", &x, &y, &color) != 3) {
				fclose(fileKey);
				fclose(fileMsg);

				// it's probably not a problem
				return key_fn;
			}
			// assert pixel position 
			if(!(x <= image->bi.biWidth && y <= abs(image->bi.biHeight))) {
				fclose(fileKey);
				fclose(fileMsg);
				return NULL;
			}
			
			// fix y
			y = abs(image->bi.biHeight) - y - 1; // image stores data from from below
			
			// get current bit
			BYTE bit = (mask & byte) >> j; 

			// change the last bit
			change_last_bit(get_byte_by_color(&image->data[x + y * image->bi.biWidth], color), bit);

			// move to the next bit 
			mask >>= 1;
		}
	}

	// success
	fclose(fileKey);
	fclose(fileMsg);
	return msg_fn;
}

// get message from image by keys
char *extract (IMAGE *image, char *key_fn, char *msg_fn) {

	// open files to read
	FILE *fileKey = fopen(key_fn, "rb");
	FILE *fileMsg = fopen(msg_fn, "wb");

	//check for null
	if (fileKey == NULL || fileMsg == NULL) {
		return NULL;
	}

	// pixel position
	int x, y;

	//pixel color
	char color;

	// bits read counter
	int cnt = 0;

	// byte to decode
	BYTE byte = 0;

	// for every key
	while (fscanf(fileKey, "%d %d %c", &x, &y, &color) == 3) {

		// assert pixel position 
		if(!(x <= image->bi.biWidth && y <= abs(image->bi.biHeight))) {
			return NULL;
		}

		//fix y
		y = abs(image->bi.biHeight) - y - 1;
		
		// we need 5 bits
		if (cnt < 5) {

			// move stored bits and get next
			byte = (byte << 1) + ((*get_byte_by_color(&image->data[x + y * image->bi.biWidth], color)) & 1);
			
			// update counter
			cnt++;
		}
		
		// if the char is read, print to file	
		if (cnt == 5) {

			//update counter
			cnt = 0;

			//print the char
			fprintf(fileMsg, "%c\n", char_by_byte(byte));

			// get ready for the next
			byte = 0;
		}
	}

	// success
	free_image(image);
	fclose(fileKey);
	fclose(fileMsg);
	return msg_fn;
}
