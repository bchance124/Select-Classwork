// Written by Ben Chance, <bchance1@vols.utk.edu>
// Lab 5, CS302
// 3 April 2018
//
// See Support.cpp for implementation and usage

#include <vector>

using namespace std;


class maze {

	public:
		void create_maze(int rows, int cols);
		void read_maze();
		void write_maze();
		void write_path();
		bool solve_maze();
		bool solve_maze(int i, int j);
		int get_Nrow() { return Nrows; }
		int get_Ncols() { return Ncols; }

		void assign(int rows, int cols) { Nrows = rows; Ncols = cols; 
			buf = new bool[(Nrows * Ncols) * 4]; }

	private:
		int Nrows;
		int Ncols;
		bool *buf;
		vector <int> path;

};

