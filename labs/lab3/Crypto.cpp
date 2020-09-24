// Ben Chance, <bchance1@vols.utk.edu>
// 27 Feb 2018
// Lab 3, CS302
//
// The purpose of this lab is to encode secret messages into an image 





#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>


using namespace std;

#include "support.h"
// randomly generates the pixel list based on the histogram of colors in the ppm file
void set_pixel_list(ppm &img, vector <pixel> &pixels) {
	vector <int> hist;
	RGB *rgb;
	int color;
	int r;
	hist.resize(4096);
	fill(hist.begin(), hist.end(), 0);
	for (int i = 0; i < img.get_Nrows(); i++) {
		for (int j = 0; j < img.get_Ncols(); j++) {
			pixel p(i, j);
			pixels.push_back(p);
			rgb = &img[i][j];
			color = (rgb->R >> 4) << 8 | (rgb->G >> 4) << 4 | (rgb->B >> 4);
			hist[color]++;
		}
	}
	rnumgen RNG;
	RNG.pdf(hist);
	for (int i = (int)hist.size() - 1; i > 0; --i) {
		r = RNG.rand() << 12 | RNG.rand();
		swap(hist[i], hist[r % (i + 1)]);
	}
}

// encodes a secret message into colors of the ppm image 
void encode(ppm &img) {
	vector <pixel> pixels;
	string text;
	pixel p;	
	char c;
	unsigned int k = 0;
	unsigned char *color;
	int it = 0;

	set_pixel_list(img, pixels);

	while (!cin.eof()) {
		c = cin.get();
		if (c == EOF) {
			c = ETX;
		}
		for (int i = 0; i < 8; i++) {

			p = pixels[k];
			RGB *rgb = &img[p.get_row()][p.get_col()];

			if (it == 0) {
				color = &rgb->R; 
				it++;
			}

			else if (it == 1) {
				color = &rgb->G;
				it++;
			}

			else {
				color = &rgb->B;
				it = 0;
			}

			*color &= 0xfe;
			*color |= ((c >> i) & 0x1);
		
			k++;
		}

	}
}
// decodes the secret message in encode by walking through the pixel list
void decode(ppm &img) {

	vector <pixel> pixels;
	string text;
	pixel p;	
	char c = '\0';
	unsigned int k = 0;
	unsigned char *color;
	int it = 0;

	set_pixel_list(img, pixels);

	while (c != ETX) {
		c = '\0';
		for (int i = 0; i < 8; i++) {

			p = pixels[k];
			RGB *rgb = &img[p.get_row()][p.get_col()];

			if (it == 0) {
				color = &rgb->R; 
				it++;
			}

			else if (it == 1) {
				color = &rgb->G;
				it++;
			}

			else {
				color = &rgb->B;
				it = 0;
			}

			*color &= 0x1;
			c |= (*color << i);
		
			k++;
		}
		cout.put(c);
	}
}

// calls the appropriate functions and error checks the inputs
int main(int argc, char *argv[]) {
	ppm img;

	if (argc != 3) {
		perror("2 arguments are required");
		exit(1);
	}

	if (!strcmp(argv[2], "-encode") && !strcmp(argv[3], "-decode")) {
		perror("Usage: ./Crypto encode|decode filename.ppm\n");
		exit(1);
	}

	img.read(argv[2]);

	//	printf("%x\n %x\n %x\n", &img[0][0], &img[0][600], &img[1][0]);


	if (!strcmp(argv[1], "-encode")) encode(img);
	else if (!strcmp(argv[1], "-decode")) decode(img);
	else printf("error\n");

	img.write(argv[2]);
}
