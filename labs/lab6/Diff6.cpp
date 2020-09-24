// Written by Ben Chance, <bchance1@vols.utk.edu>
// 17 April 2018
// Lab 6, CS302
//
// The purpose of this lab is to make a simple diff program similar to the Unix command diff


#include <iostream>
#include <fstream>
#include <cstdio>
#include <stack>
#include <vector>
#include <string>


using namespace std;

template <typename T>
class matrix {
	// see handout from class or make your own (this is handout from class)
	public:
		~matrix() { delete [] buf; }

		void assign(int rows, int cols) { buf = new T[rows*cols]; Nrows = rows, Ncols = cols; }

		T * operator[] (int i) { return &buf[i * Ncols]; }
		int size() const { return Nrows * Ncols; }
	private:
		int Nrows, Ncols;
		T *buf;
};

// Longest Common Subsequence
class LCS {
	public:
		void text1_push_back(string a) { ex.push_back(a); } // file 1
		void text2_push_back(string a) { ey.push_back(a); } // file 2
		void compute_alignment();
		void report_difference();
		
	private:
		// support functions

		// text1 buffer
		vector <string> ex;

		// text2 buffer
		vector <string> ey;

		matrix <int> cost;
		matrix <int> link;

		void report_difference(int m, int n);
		vector <string> operation; 	

};

// taken from dpalign handout on Canvas
void LCS::compute_alignment() {

	ex.insert(ex.begin(), "-");
	ey.insert(ey.begin(), "-");
		
	int m = ex.size() - 1;
	int n = ey.size() - 1;

	cost.assign(m + 1, n + 1);
	link.assign(m + 1, n + 1);

	cost[0][0] = 0;
	link[0][0] = 0;

	for (int i = 1; i <= m; i++) {
		cost[i][0] = cost[i-1][0] + 1;
		link[i][0] = 1;
	}

	for (int j = 1; j <= n; j++) {
		cost[0][j] = cost[0][j-1] + 1;
		link[0][j] = 2;
	}

	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			cost[i][j] = cost[i-1][j-1] + (ex[i] == ey[j] ? 0 : m+n);
			link[i][j] = 4;


			int inscost = cost[i][j-1] + 1;
			if (inscost < cost[i][j]) {
				cost[i][j] = inscost;
				link[i][j] = 2;
			}

			int delcost = cost[i-1][j] + 1;
			if(delcost < cost[i][j]) {
				cost[i][j] = delcost;
				link[i][j] = 1;
			}			
		}
	}
}

// Calls recursive report difference from bottom right of matrix, then prints
void LCS::report_difference() {
	report_difference(ex.size()-1, ey.size()-1);

	// clears buffer of operations if needed
	if (operation[operation.size() - 1] != "SUB") operation.insert(operation.begin(), "SUB");

	int ex_line = 0;
	int ey_line = 0;
	int num_del = 0;
	int num_ins = 0;

	// goes through list of operations generated in report difference
	for (int i = (int)operation.size() - 1; i >=0; i--) {
	
		if (operation[i] == "SUB") {
			// match first
			if (num_del == 1 && num_ins == 1) {
				printf("%dc%d\n", ex_line, ey_line);
				printf("< %s\n---\n> %s\n", ex[ex_line].c_str(), ey[ey_line].c_str());
			}

			if (num_del > 1 && num_ins > 1) {
				printf("%d,%dc%d,%d\n", ex_line - num_del + 1, ex_line, ey_line - num_ins + 1, ey_line); 
				for (int j = ex_line - num_del + 1; j <= ex_line; j++) 
					printf("< %s\n", ex[j].c_str());
				printf("---\n");
				for (int j = ey_line - num_ins + 1; j <= ey_line; j++) 
					printf("> %s\n", ey[j].c_str());
			}
			// then insert
			if (num_del == 0 && num_ins != 0) {
				if (num_ins == 1) printf("%da%d\n", ex_line, ey_line);
				else printf("%da%d,%d\n", ex_line, ey_line - num_ins + 1, ey_line);
				for (int j = ey_line - num_ins + 1; j <= ey_line; j++)
					printf("> %s\n", ey[j].c_str());
				
			}	
			// delete third
			if (num_del != 0 && num_ins == 0) {
				if (num_del == 1) printf("%dd%d\n", ex_line, ey_line);
				else printf("%d,%dd%d\n", ex_line - num_del + 1, ex_line, ey_line);
				for (int j = ex_line - num_del + 1; j <= ex_line; j++) 
					printf("< %s\n", ex[j].c_str());
			}
	
			ex_line++;
			ey_line++;

			num_del = 0;
			num_ins = 0;
		}

		if (operation[i] == "INS") {
			ey_line++;
			num_ins++;
		}

		if (operation[i] == "DEL") {
			ex_line++;
			num_del++;
		}

	}
}

// pushed back operations given by link matrix
void LCS::report_difference(int m, int n) {
	if (m == 0 && n == 0) return;

	if (link[m][n] == 1) {
		operation.push_back("DEL");
		report_difference(m - 1, n);
	}

	if (link[m][n] == 2) {
		operation.push_back("INS");
		report_difference(m, n - 1);
	}

	if (link[m][n] == 4) {
		operation.push_back("SUB");
		report_difference(m - 1, n - 1);
	}
}



int main(int argc, char *argv[])
{
	string line;


	// check two input files are specified on command line
	if (argc != 3) printf("Usage program file1 file2\n");

	LCS lcs;  // instantiate your "lcs based diff" object

	// read the text from file1 into the lcs.text1 buffer
	ifstream infile1;
	infile1.open(argv[1]);
	while (getline(infile1, line)) {
		lcs.text1_push_back(line);
	}



	// read the text from file2 into the lcs.text2 buffer
	ifstream infile2;
	infile2.open(argv[2]);
	while(getline(infile2, line)) {
		lcs.text2_push_back(line);
	}


	lcs.compute_alignment();
	lcs.report_difference();
}
