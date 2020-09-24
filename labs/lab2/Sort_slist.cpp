// Written by Ben Chance, <bchance1@vols.utk.edu>
// Lab 2, CS302 UTK
// 12 Feb 2017
//
// The purpose of this lab is to implement sorting using smart pointers for data manipulation

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <algorithm>
#include <iomanip>
#include <cstdlib>



using namespace std;

// slist is used for node, smart pointers, and iterators
template <class T>
class slist {
	private:
		struct node {
			node() { data = T(); next = NULL; }
			//add node(const T &key) { write this }
			node(const T &key) {
				data = key;
				next = NULL;
			}
			// add overloaded operator< code
			bool operator<(const node &n) const {
				if (data < n.data) return true;
				else return false;
			}

			T data;
			node *next;
		};

		// smart pointer class for sorting data
		class sptr {
			public: 
				sptr(node *_ptr=NULL) {ptr = _ptr; }
				
				bool operator< (const sptr &rhs) const {
					return *ptr < *rhs.ptr;
				}

				operator node * () const { return ptr; }

			private:
				node *ptr;
		};

	public:
		// iterator can count number of elements to be sorted
		class iterator {
			public:
				iterator() { p = NULL; }
				T & operator*() { return p->data; }
				iterator & operator++() { p = p->next; return *this; }
				bool operator!=(const iterator & rhs) const { return p != rhs.p; }

				friend class slist<T>;

			private:
				iterator(node *n_p) { p = n_p; }
				node *p;
		};

	public:
		slist();
		~slist();

		void push_back(const T &);
		void sort();

		iterator begin() { return iterator(head->next); }
		iterator end() { return iterator(NULL); }

	private:
		node *head;
		node *tail;
};

template <typename T>
slist<T>::slist() {
	head = new node();
	tail = head;
}

template <typename T>
slist<T>::~slist() {
	while (head->next != NULL) {
		node *p = head->next;
		head->next = p->next;
		delete p;
	}
	delete head;

	head = NULL;
	tail = NULL;
}

template <typename T>
void slist<T>::push_back(const T &din) {
	tail->next = new node(din);
	tail = tail->next;
}

template <typename T>
void slist<T>::sort() {
	iterator lit = begin();
	int size = 0;
	node *p;
	// determine number of list elements
	while (lit != end()) {
		++size;
		++lit;
	}
 	// set up smart pointer array called Ap
	vector <sptr> Ap;
	Ap.resize(size);

	p = head;
	for (int i = 0; i < size; i++) {
		Ap[i] = p->next;
		p = p->next;
	}

	// apply std::sort(Ap.begin(), Ap.end())
	std::sort(Ap.begin(), Ap.end());
	// use sorted Ap array to relink list 
	p = head;
	for (int i = 0; i < size; i++) {
		p->next = Ap[i];
		p = Ap[i];
	}
	
	p->next = NULL;
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
	//compares last name, first name, and phone number in order for sorting
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
	// copy command-line check from Quicksort.cpp

	slist<person_t> A;

	person_t din;
	while (cin >> din)
		A.push_back(din);

	A.sort();

	printlist(A.begin(), A.end());
}
