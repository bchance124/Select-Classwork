// Written by Ben Chance, <bchance1@vols.utk.edu>
// 19 March 2018
// CS 302, Spring 2018
// Lab 4
//
// The purpose of this lab is to give the user options to gain information on cities and travel paths to those cities

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <queue>
#include <stack>
#include <limits>
#include <map>
#include <cstring>

using namespace std;

// stores general information on the city 
class city {

	public:

		// public getter functions for each private variable
		string get_name() const { return name; }
		string get_type() const { return type; }
		int get_zone() const { return zone; }
		float get_latitude() const { return latitude; }
		float get_longitude() const { return longitude; }
		int get_population() const { return population; }

		// used in read and write cityinfo
		friend ifstream& operator >> (ifstream &fin, city &c) { fin >> c.zone >> c.name >> c.type >> c.latitude >> c.longitude >> c.population; return fin; }
		friend ofstream& operator << (ofstream &fout, city &c) { fout << setw(20) << left << c.name << setw(12) << c.type << setw(2) << c.zone << setw(10) << right << c.population << setw(8) << fixed <<  setprecision(2) << c.latitude << setw(8) << fixed << setprecision(2) <<  c.longitude << "\n"; return fout; }


	private:
		string name;
		string type;
		int zone;
		float latitude;
		float longitude;
		int population;
};

// class to store and calculate distance between any two cities
template <typename T>
class dtable {
	public:
		dtable() { buf = NULL; }
		//~dtable();
		void assign(int size) { N = size; buf = new T[(N * (N + 1)) / 2]; }
		T *operator[] (int i) { return &buf[(i * (i + 1)) / 2]; }
		T *data() { return buf; }
		int size() { return N; }
		T operator() (int i, int j) { if (i > j) return buf[((i*(i+1))/2)+j]; return buf[((j*(j+1))/2)+i]; }

	private:
		int N;
		T *buf;
};

// creates the adjacency list used for path calculations
void create_citygraph(vector <vector<int> > &adj, dtable <float> &dist, vector <city> &cities) {

	vector <int> regs;
	vector <int> gates;
	vector <unsigned int> rzones, gzones;

	float min_val = 25000.0; // about the equator of the earth
	unsigned int min_ind;

	unsigned int i, j, k;

	unsigned int num_zones = 6;
	float max_gate_dist = 6000.0;

	adj.resize(cities.size());

	// creates a regional and gateway list for each type of city
	for (i = 0; i < cities.size(); i++) {
		if (cities[i].get_type() == "GATEWAY") {
			gates.push_back(i);
			gzones.push_back(cities[i].get_zone());
		}
		else {
			regs.push_back(i);
			rzones.push_back(cities[i].get_zone());
		}
	}

	// regionals to regionals
	for (i = 0; i < regs.size(); i++) {
		for (j = i+1; j < regs.size(); j++) {
			if (rzones[i] == rzones[j]) {
				adj[regs[i]].push_back(regs[j]);
				adj[regs[j]].push_back(regs[i]);
			}
		}
	}

	// regionals to gateways
	for (i = 0; i < regs.size(); i++) {
		min_val = 25000;
		for (j = 0; j < gates.size(); j++) {
			if (rzones[i] == gzones[j]) {
				if (dist(regs[i], gates[j]) < min_val) {
					min_val = dist(regs[i], gates[j]);
					min_ind = gates[j];
				}
			}
		}
		adj[regs[i]].push_back(min_ind);
		adj[min_ind].push_back(regs[i]);
	}

	// gateways to gateways in same zone
	for (i = 0; i < gates.size(); i++) {
		for (j = i+1; j < gates.size(); j++) {
			if (gzones[i] == gzones[j]) {
				adj[gates[i]].push_back(gates[j]);
				adj[gates[j]].push_back(gates[i]);
			}
		}
	}

	// gateways to gateways in other zones
	for (i = 0; i < gates.size(); i++) {
		for (k = 1; k <= num_zones; k++) {
			min_val = 25000;
			min_ind = -1;
			for (j = 0; j < gates.size(); j++) {
				if (gzones[j] == k && k != gzones[i]) {
					if (dist(gates[i], gates[j]) < min_val) {
						min_val = dist(gates[i], gates[j]);
						min_ind = gates[j];
					}
				}
			}

			if (min_val <= max_gate_dist) {
				adj[gates[i]].push_back(min_ind);
				adj[min_ind].push_back(gates[i]);
			}
		}
	}
}

// stores city info in vector and names and indices in map
void read_cityinfo(vector <city> &cities, map <string, int> &city_lookup) { 
	ifstream fin;
	
	fin.open("citylist.txt");
	if (fin.fail())  cerr << "Could not open citylist.txt\n";
	int i = 0;
	city c;
	while (!fin.eof()) {
		while(fin >> c) {

			cities.push_back(c);
			city_lookup.insert(make_pair(c.get_name(), i));
			i++;
		}
		fin.clear();
		fin.ignore(1024, '\n');
	}
	fin.close();
}

// writes info to cityinfo.txt
void write_cityinfo(vector <city> &cities) {
	ofstream fout("cityinfo.txt");
	if (fout.fail()) cerr << "Could not open cityinfo.txt\n";
	fout << "CITY INFO (N=" << cities.size() << "):\n\n";


	for (unsigned int i = 0; i < cities.size(); i++) {
		fout << setw(3) << right << i << " ";
		fout << cities[i];
	}
	fout.close();
}

// fills distance table for use in path calculations
void write_citydtable(vector <city> &cities, dtable <float> &dist, bool print) { 
	float pi = M_PI;
	int radius = 3982;
	float lat1;
	float lat2;
	float lon1;
	float lon2;
	unsigned int max = 0;

	

	dist.assign(cities.size());

	for (unsigned int i = 1; i < cities.size(); i++) {
		for (unsigned int j = 0; j < i; j++) {
			lat1 = cities[(int)i].get_latitude() * pi/180;
			lat2 = cities[(int)j].get_latitude() * pi/180;
			lon1 = cities[(int)i].get_longitude() * pi/180;
			lon2 = cities[(int)j].get_longitude() * pi/180;

			// distance formula given latitude and longitude
			dist[i][j] = 5.0 * round(2 * asin( sqrt( pow( sin( abs( (lat1-lat2) / 2 ) ) , 2 ) + cos(lat1) * cos(lat2) * pow( sin( (lon1-lon2) / 2 ) , 2 ) ) ) * radius / 5.0 ) ;
		}

	}

	// only prints if -write_citydtable
	if (print) {

		ofstream fout("citydtable.txt");
		fout << "DISTANCE TABLE:\n\n";

		for (unsigned int i = 0; i < cities.size(); i++)if (cities[i].get_name().length() > max) max = cities[i].get_name().length();
		max = (max*2) + 4;

		for (unsigned int i = 1; i < cities.size(); i++) {
			for (unsigned int j = 0; j < i; j++) {
				
				string names = cities[i].get_name() + " to " + cities[j].get_name() + " ";
				fout << setfill(' ') << setw(3) << right << i << " " << names << setfill('.') << setw(max - names.length()) << "" << right << setfill(' ') << setw(5) << dist(i,j) << " miles\n";
			}
			fout << "\n";
		}
	}
}

// writes adjacency list to citygraph.txt
void write_citygraph(vector <vector<int> > &adj, dtable <float> &dist, vector <city> &cities) {

	ofstream fout("citygraph.txt");
	int tmp = -1;
	unsigned int i, j;

	fout << "CITY GRAPH:\n\n";

	//for (i = 0; i < adj.size(); i++)
	//	sort(adj[i].begin(), adj[i].end());

	for (i = 0; i < adj.size(); i++) {
		for (j = 0; j < adj[i].size(); j++) {
		}
	}

	for (i = 0; i < adj.size(); i++) {
		fout << setw(3) << i << " "  << cities[i].get_name() << "\n";
		for (j = 0; j < adj[i].size(); j++) {
			if (tmp != adj[i][j] && adj[i][j] < 30) {
				tmp = adj[i][j];
				fout << setw(6) << right;
				fout << adj[i][j] << " " << cities[adj[i][j]].get_name() << ": " << dist(i, adj[i][j]) << " miles\n";
			}
		}
		if (i != adj.size() -1 )fout << "\n";
	}			
}


// caclutes shortest travelled distance between two cities using bfs or dijkstra algorithm
void shortest_route(int source, int sink, int method, vector <vector <int> > &adj, dtable <float> &dist, vector <city> &cities, int show) {


	vector <int> vdist;
	vector <int> vlink;

	// bfs
	if (method == 0) {
		vdist.assign(cities.size(), numeric_limits<int>::max()); // ~circumfrence of earth
		vlink.assign(cities.size(), -1);

		vdist[source] = 0;
		vlink[source] = source;

		queue <int> Q;
		Q.push(source);

		while(!Q.empty()) {
			int i = Q.front();
			Q.pop();

			if (i == sink)
				break;
			for (int k = 0; k < (int)adj[i].size(); k++) {
				int j = adj[i][k];
				if (vdist[j] == numeric_limits<int>::max()) {
					vdist[j] = vdist[i] + (int)dist(i, j);
					vlink[j] = i;
					Q.push(j);
				}
			}
		}

		while(!Q.empty())
			Q.pop();

	}

	// dijkstra
	else if (method == 1) {
		vector <bool> vcolor;

		vcolor.assign(cities.size(), false);

		vdist.assign(cities.size(), numeric_limits<int>::max());
		vdist[source] = 0;

		vlink.assign(cities.size(), -1);
		vlink[source] = source;

		while(1)  {
			int i;
			int i_mindist = -1;

			float mindist = numeric_limits<float>::max();

			for (int  i = 0; i <(int)vcolor.size();  i++) {
				if  (vcolor[i] == false && mindist > vdist[i]) {
					i_mindist = i;
					mindist = vdist[i];
				}
			}


			if ((i = i_mindist) == -1) return;

			vcolor[i] = true;

			if (i == sink)  break;

			for (int k = 0; k < (int)adj[i].size(); k++) {
				int j = adj[i][k];
				float wij = dist(i, j);
				if (vcolor[j] ==  false) {
					if  (vdist[j] > vdist[i] + wij) {
						vdist[j] = vdist[i] + wij;
						vlink[j] = i;
					}
				}
			}
		}
	}
	if (vdist[sink] == numeric_limits<int>::max()) {
		cout << "No path from " << cities[source].get_name() << " to " << cities[sink].get_name() << "\n";
		return;
	}

	// only print full list if -show, same for bfs and dijkstra
	if (show == 1) {

		stack<int> S;

		for (int i = sink; i != source; i =vlink[i])
			S.push(i);
		S.push(source);
		int tmp = - 1;
		while(!S.empty()) {
			int i = S.top();
			S.pop();
			if (i == source) cout << setw(8) << vdist[i] << " miles :" << setw(3) << i << " " << setw(20) << left << cities[i].get_name();
			else cout << setw(8) << right << vdist[i] << " miles :" << setw(3) << i << " " << setw(23) << left << cities[i].get_name() << right << setw(5) << (int)dist(i, tmp) << " miles";
			cout << "\n";
			tmp = i;
		}
	}

	else {
		cout << right << setw(2) << source << " " << cities[source].get_name() << " to" << setw(3) << right << sink << " " << cities[sink].get_name() << " : " << vdist[sink] << " miles\n";
	}
		
}

int main(int argc, char *argv[])
{
	
	vector <city> cities;
	dtable <float> dist;
	vector <vector<int> > adj;
	map <string, int> city_lookup;
	int i, show = 0;
	string source, sink;
	bool print = 0;


	read_cityinfo(cities, city_lookup);
	write_citydtable(cities, dist, false);
	create_citygraph(adj, dist, cities);

	for (i = 0; i < (int)adj.size(); i++)
		sort(adj[i].begin(), adj[i].end());


	for (i = 1; i < argc; i++) 
		if (strcmp(argv[i], "-show") == 0) show = 1;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-write_info") == 0) 
			write_cityinfo(cities);
		if (strcmp(argv[i], "-write_dtable") == 0) 
			write_citydtable(cities, dist, true);
		if (strcmp(argv[i], "-write_graph") == 0)
			write_citygraph(adj, dist, cities);
		
		if (strcmp(argv[i], "-mode_bfs") == 0)
			while(!cin.eof()) {
				print = true;
				cout << "Enter> ";
				cin >> source >> sink;
				if (cin.eof()) { cout << "\n"; break; }
				if ((city_lookup.upper_bound(source)->first.find(source) == string::npos)) {
					cout << source << " unknown\n";
					print = false;
				}
				if ((city_lookup.upper_bound(sink)->first.find(sink) == string::npos)) {
					cout << sink << " unknown\n";
					print = false;
				}
				if (print == true)
					shortest_route(city_lookup.upper_bound(source)->second, city_lookup.upper_bound(sink)->second, 0, adj, dist, cities, show);

				cout << "\n";
				
			}
		if (strcmp(argv[i], "-mode_dijkstra") == 0) {
			while(!cin.eof()) {
				print = true;
				cout << "Enter> ";
				cin >> source >> sink;
				if (cin.eof()) { cout << "\n"; break; }
				if ((city_lookup.upper_bound(source)->first.find(source) == string::npos)) {
					cout << source << " unknown\n";
					print = false;
				}
				if ((city_lookup.upper_bound(sink)->first.find(sink) == string::npos)) {
					cout << sink << " unknown\n";
					print = false;
				}
				if (print == true)
					shortest_route(city_lookup.upper_bound(source)->second, city_lookup.upper_bound(sink)->second, 1, adj, dist, cities, show);

				cout << "\n";
			}
		}
	}
}
