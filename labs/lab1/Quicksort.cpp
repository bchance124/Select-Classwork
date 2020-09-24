// Ben Chance, <bchance1@vols.utk.edu>
// CS 302, 29 Jan 2018
// Lab 1
//
// The purpose of this lab is to implement quicksort using the random pivot method.

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

template <typename Tdata>
void quickselect (vector <Tdata> &A, int left, int k, int right) {
	while (1) {
		if (right <= left) return;
		int pindex =  left + rand() % (right - left + 1);
		Tdata pivot = A[pindex];
		// move pivot to end of set
		swap(A[pindex], A[right]);
		int i = left;
		int j = right;
		// breaks out when necessary
		while(1) {

			// selects left element for swap
			while(A[i] < pivot && i < j){
				i++;
				if (i == A.size()) {
					break;
				}
			}

			// selects right element for swap
			while(pivot < A[j] && i < j){
				j--;
				if (j == -1) {
					break;
				}
			}
			// swaps two previous
			if (i < j) {
				swap(A[i], A[j]);
			}
			else break;
		}

		// swaps pivot back into correct position
		pindex = i;
		swap(A[pindex], A[right]);
		// element is in right spot
		if (pindex == k) return;

		if (k < pindex) right = pindex - 1;
		else			left = pindex + 1;
	}
}

// This function recursively sorts data given in vector A
template <typename Tdata>
void quicksort(vector <Tdata> &A, int left, int right) { 

	// base case
	if (right <= left) return;
	// selects random element from unsorted data
	int pindex =  left + rand() % (right - left + 1);
	Tdata pivot = A[pindex];
	// move pivot to end of set
	swap(A[pindex], A[right]);
	int i = left;
	int j = right;
	// breaks out when necessary
	while(1) {

		// selects left element for swap
		while(A[i] < pivot && i < j){
			i++;
			if (i == A.size()) {
				break;
			}
		}

		// selects right element for swap
		while(pivot < A[j] && i < j){
			j--;
			if (j == -1) {
				break;
			}
		}
		// swaps two previous
		if (i < j) {
			swap(A[i], A[j]);
		}
		else break;
	}

	// swaps pivot back into correct position
	pindex = i;
	swap(A[pindex], A[right]);

	if (left < right) {
		quicksort(A, left, pindex - 1);
		quicksort(A, pindex + 1, right);
	}
	return;
}

class person_t {
	public:
		person_t() { }

		// add operator< using lastname, firstname, phone number

		friend istream & operator>>(istream &, person_t &);
		friend ostream & operator<<(ostream &, const person_t &);
		bool operator<(const person_t &) const;

	private:
		string firstname;
		string lastname;
		string phonenum;
};

istream & operator>>(istream &in, person_t &r) { 
	// reads person_t object data
	in >> r.firstname;
	in >> r.lastname;
	in >> r.phonenum;
	return in;
}

ostream & operator<<(ostream &out, const person_t &r) {
	// write this to write person_t object data
	string name = r.lastname + " " + r.firstname;
	out << setw(24) << left << name << r.phonenum;
	return out;
}

bool person_t::operator<(const person_t &p2) const {
	// compares last name, first name, and phone number in order for sorting
	if (lastname < p2.lastname) return true;
	if (p2.lastname < lastname) return false;
	if (firstname < p2.firstname) return true;
	if (p2.firstname < firstname) return false;
	if (phonenum < p2.phonenum) return true;
	if (p2.phonenum < phonenum) return false;
	return true;
}

template <typename T>
void printlist(T p1, T p2) { 
	while (p1 != p2) {
		cout << *p1 << "\n";
		++p1;
	}
}

int main(int argc, char *argv[]) {
	// perform command-line check 

	vector<person_t> A;

	person_t din;
	while (cin >> din)
		A.push_back(din);

	srand(time(NULL));

	int N = (int)A.size();

	int k0 = 0;
	int k1 = N-1;

	// if given as command-line arguments,
	// update k0, k1 and apply quickselect
	// to partition A accordingly

	// Quickselect Mode
	if (argc == 3) {
		k0 = atoi(argv[1]);
		k1 = atoi(argv[2]);
		quickselect(A, 0, k0, N - 1);
		quickselect(A, k0, k1, N-1);
	}
	// Command Line Argument Error
	else if (argc != 1) {
		printf("Usage 1: Quicksort\nUsage 2: Quicksort k0 k1\n");
		exit(1);
	}

	quicksort(A, k0, k1);

	printlist(A.begin(), A.end());
}
