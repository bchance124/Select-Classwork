#include <iostream>
#include <fstream>
#include <cstdlib>
#include <functional>
#include <algorithm>
#include <numeric>


using namespace std;

#include "support.h"

const char ETX = 0x3;
// reads the image file into the appropriate data structures
void ppm::read(string filename) { 
	ifstream fs;
	string line;
	int cols, rows, max;
	
	fs.open(filename.c_str());
	if (!fs.is_open()) {
		perror("Could not open file\n");
		exit(1);
	}
	
	fs >> line;
	
	if (line != "P6") {
		cerr << line << ":";
		perror(" Not a P6 File\n");
		exit(1);
	}
	
	fs >> cols;
	fs >> rows;

	fs >> max;

	if (max != 255) {
		cerr << max << ":";
		perror(" Max Val must be 255\n");
		exit(1);
	}

	image.assign(rows, cols);

	while(fs.get() != '\n') {}

	fs.read((char *) image.data(), 3 * rows * cols);
	if (fs.gcount() != (3 * rows * cols)) {
		perror("File size differs from header\n");
		exit(1);
	}

	fs.close();	
}

// writes out the image files that includes an encoded message
void ppm::write(string filename) { 
	ofstream fs;
	filename = filename.substr(0, filename.length() - 4);
	filename += "_wmsg.ppm";
	fs.open(filename.c_str());
	if (!fs.is_open()) {
		perror("Could not open file\n");
		exit(1);
	}
	
	fs << "P6\n";
	fs << image.get_Ncols() << " " << image.get_Nrows() << "\n";
	fs << "255\n";


	fs.write((char *) image.data(), 3 * image.get_Nrows() * image.get_Ncols());
	fs.close();

}

void rnumgen::pdf(const vector <int> &v) {
	F.resize(v.size());
	partial_sum(v.begin(), v.end(), F.begin());
	transform(F.begin(), F.end(), F.begin(), bind2nd(divides<float>(), *(F.end() - 1)));
}

int rnumgen::rand() const {
	const float randnorm = RAND_MAX + 1.0f;
	const float p = (float)std::rand()/randnorm;
	return upper_bound(F.begin(), F.end(), p) - F.begin();
}
