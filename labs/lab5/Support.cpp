// Written by Ben Chance, <bchance1@vols.utk.edu>
// Lab 5, CS302
// 3 April 2018
//
// The purpose of this lab is to create a maze by printing wall indices or solve a maze given wall indices


#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <stack>
#include "Support.h"
#include "Dset.h"

// used for pairs and directions when creating the maze
class wall {
	public:
	
		wall(int i = -1, int j = -1, int k = -1) { first = i; second = j; dir = k; }
		int first, second;
		int dir;
};


// creates the maze of bools with 4 walls to a cell
void maze::create_maze(int rows, int cols) {

	assign(rows, cols);

	vector <wall> walls;	
	
	wall w;
	// adds interior wall pairs to walls vector
	for (int i = 0; i < Nrows; i++) {
		for (int j = 0; j < Ncols; j++) {
			for (int k = 0; k < 4; k++) {
				buf[(i * Ncols * 4) + (j * 4) + k] = true;
			} 
				if ( i != Nrows - 1) {
					w.first = i; w.second = j; w.dir = 2;
					walls.push_back(w);
				}
				if ( j != Ncols - 1) {
					w.first = i; w.second = j; w.dir = 3;
					walls.push_back(w);
				}
		}
	}

	// random permuation 
	for (int i = (int)walls.size() - 1; i > 0; --i) {
		swap(walls[i], walls[rand() % (i + 1)]);
	}

	// Disjoint Set
	dset DS(Nrows * Ncols);
	
	// easier calculation variables
	int r1, r2, c1, c2, w1, w2;

	for (int i = 0; i < (int) walls.size(); i++) {
		if (DS.size() == 1) break; // all cells are connected
		r1 = walls[i].first; // row
		c1 = walls[i].second; // column
		w1 = walls[i].dir;
		// North
		if (walls[i].dir == 0) {
			r2 = r1 - 1;
			c2 = c1;
			w2 = 2;
		}
		// East
		if (walls[i].dir == 1) {
			r2 = r1;
			c2 = c1 - 1;
			w2 = 3;
		}
		// South
		if (walls[i].dir == 2) {
			r2 = r1 + 1;
			c2 = c1;
			w2 = 0;
		}
		// West
		if (walls[i].dir == 3) {
			r2 = r1;
			c2 = c1 + 1;
			w2 = 1;
		}
		
		// merge if not connected
		if (DS.find((r1 * Ncols) + (c1)) != DS.find((r2 * Ncols) + (c2)) ) {
			DS.merge((r1 * Ncols ) + (c1 ) , (r2 * Ncols) + (c2));
			buf[(r1 * Ncols * 4) + (c1 * 4) + w1] = false;
			buf[(r2 * Ncols * 4) + (c2 * 4) + w2] = false;
		}
	}
}

// simply prints out walls of maze 
void maze::write_maze() {
	cout << "MAZE " << Nrows << " " << Ncols << "\n";

	for (int i = 0; i < Nrows; i++) {
		for (int j = 0; j < Ncols; j++) {
			for (int k = 0; k < 4; k++) {
				if (buf[(i * Ncols * 4) + (j * 4) + k] == true) {
					cout << i * Ncols + j << " " << k << endl;
				}
			}
		}
	}
}

// default case for solve_maze
bool maze::solve_maze() { return solve_maze(0, Ncols * Nrows -1); }

// finds a dfs path from source index to sink index
bool maze::solve_maze(int source, int sink) { 
	
	vector <bool> visited;
	int i = -1;

	visited.assign(Nrows * Ncols, false);
	path.assign(Nrows * Ncols, -1);

	stack<int>S;
	S.push(source);

	while(!S.empty()) {
		if (i == sink) break;
		i = S.top();
		S.pop();
		if (visited[i] == true) continue;

		visited[i] = true;

		// k is adjency list for each index
		for (int k = 0; k < 4; k++) {
			if (buf[(i * 4) + k] == false) {
				// North
				if (k == 0 && visited[i-Ncols] == false) {
					S.push(i - Ncols);
					path[i - Ncols] = i;
				}
				// East
				if (k == 1 && visited[i-1] == false) {
					S.push(i - 1);
					path[i - 1] = i;
				}
				// South
				if (k == 2 && visited[i+Ncols] == false) {
					S.push(i + Ncols);
					path[i + Ncols] = i;
				}
				// West
				if (k == 3 && visited[i+1] == false) {
					S.push(i + 1);
					path[i + 1] = i;
				}
			}
		}
	}

	path.push_back(source); // used for writing path
	path.push_back(sink);

	// route exists from source to sink
	if (visited[sink] == true)
		return true;
	// route does not exist from source to sink
	return false;
}

// builds maze given walls
void maze::read_maze() {
	string junk;
	cin >> junk >> Nrows >> Ncols;


	int i, k;

	assign(Nrows, Ncols);

	for (int i = 0;  i < Nrows; i++) {
		for (int j = 0; j < Ncols; j++) {
			for (int k = 0; k < 4; k++) {
				buf[(i * Ncols * 4) + (j * 4) + k] = false;
			}
		}
	}

	while(!cin.eof()) {
		cin >> i >> k;
		buf[(i * 4) + k] = true;
	}
}

// prints out path given by solve_maze if  path exists
void maze::write_path() {

	int source = path[path.size() - 2];
	int sink = path[path.size() - 1];
	path.pop_back(); path.pop_back();

	cout << "PATH " << Nrows << " " << Ncols << endl;

	stack <int> S;

	for (int i = sink; i != source; i = path[i]) S.push(i);
	S.push(source);

	while (!S.empty()) {
		int i = S.top();
		S.pop();
		cout << i << endl;

	}
}
