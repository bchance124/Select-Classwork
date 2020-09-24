#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
typedef unsigned char uchar;
extern const char ETX;

// allows 2D access to a 1D matrix
template <typename T>
class matrix {
	public:
		matrix() { buf = NULL; }

		~matrix() {
			delete [] buf;
		}

		int get_Nrows() const { return Nrows; }
		int get_Ncols() const { return Ncols; }
		
		void assign(int rows, int cols) {buf = new T [rows * cols]; Nrows = rows; Ncols = cols; }
		T * data() { return buf; }

		T * operator[] (int i) { return &buf[i*Ncols]; }

	private:
		int Nrows, Ncols;
		T *buf;
};

// stores rows and columns indices
struct pixel {
	public: 
		pixel(int n_row = 0, int n_col = 0) { row = n_row; col = n_col; }
		int get_row() { return row; }
		int get_col() { return col; }

	private:
		int row, col;
};

// holds the color of each pixel
struct RGB { 
	public:
		RGB(unsigned char r = 0, unsigned char g = 0,unsigned char b = 0) {R = r; G = g; B = b; }

	
		unsigned char R, G, B;
};

// holds a matrix containing the image information
class ppm { 
	public:
		ppm() { image.assign(0, 0); }
		void read(string filename);
		void write(string filename);
		RGB * operator[] (int i) { return image[i]; }
		int get_Nrows() { return image.get_Nrows(); }
		int get_Ncols() { return image.get_Ncols(); }

	private:
		matrix <RGB> image;
};

// used for generating a random pixel list
class rnumgen {
	public:
		rnumgen(int seed = 0) { srand(seed); }
		void pdf(const std::vector<int> &v);
		int rand() const;

	private:
		std::vector<float> F;
};

#endif
