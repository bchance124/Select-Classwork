// Written by Ben Chance, <bchance1@vols.utk.edu>
// Lab 7, CS302
// 29 April 2018
//
// The purpose of this lab it to turn data from xml to human readable


#include <cstdio>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

#include "Person.h"
#include "Sptrsort.h"

// reads in data from command line and calls support functions
int main(int argc, char *argv[]) {
	if (argc != 2) printf("Use 1 input xml file\n");

	ifstream fin;
	fin.open(argv[1]);

	person *n_person;
	vector<person *> person_list;

	person_enum person_type = UNKNOWN;

	int line = 0;
	size_t iL, iR;
	string input;
	string name;
	string category;
	vector<string> course;
	vector<float> gp;
	float new_gp;
	// reads in the file
	while (getline(fin, input)) {
		line++;

		if ((iL = input.find('<')) == string::npos) {
			continue;

		} else if (input.compare(iL,9,"<faculty>") == 0) {
			person_type = FACULTY;
			course.clear();
			gp.clear();

		} else if (input.compare(iL,10,"</faculty>") == 0) {
			faculty_enum faculty_type;

			if (category.compare("Assistant Professor") == 0)
				faculty_type = ASST_PROF;
			else if (category.compare("Associate Professor") == 0)
				faculty_type = ASSOC_PROF;
			else if (category.compare("Full Professor") == 0)
				faculty_type = FULL_PROF;

			//	  CODE FOR ADDING FACULTY PERSON TO DATABASE
			n_person = new faculty(name, faculty_type, course);

			person_list.push_back(n_person);

			person_type = UNKNOWN;
			continue;

		} else if (input.compare(iL,9,"<student>") == 0) {
			person_type = STUDENT;
			course.clear();
			gp.clear();

		} else if (input.compare(iL,10,"</student>") == 0) {
			student_enum student_type;

			if (category.compare("Freshman") == 0)
				student_type = FRESHMAN;
			else if (category.compare("Sophomore") == 0)
				student_type = SOPHOMORE;
			else if (category.compare("Junior") == 0)
				student_type = JUNIOR;
			else if (category.compare("Senior") == 0)
				student_type = SENIOR;

			//	  CODE FOR ADDING STUDENT PERSON TO DATABASE
			n_person = new student(name, student_type, course, gp);

			person_list.push_back(n_person);

			person_type = UNKNOWN;
			continue;

		} else if (input.compare(iL,5,"<name") == 0) {
			iL = input.find("=\"", iL);
			iR = input.find("\"/>", iL+2);
			name = input.substr(iL+2,iR-(iL+2));

		} else if (input.compare(iL,10,"<category=") == 0) {
			iL = input.find("=\"", iL);
			iR = input.find("\"/>", iL+2);
			category = input.substr(iL+2,iR-(iL+2));

		} else if (input.compare(iL,7,"<course") == 0) {
			iL = input.find("=\"", iL);
			iR = input.find("\"", iL+2);
			course.push_back(input.substr(iL+2,iR-(iL+2)));

			iL = iR;
			if (person_type == FACULTY) {
				iR = input.find("/>", iL+1);
			} else if (person_type == STUDENT) {
				iL = input.find("gp=\"", iL);
				iR = input.find("\"/>", iL+4);

				stringstream ss;
				ss << input.substr(iL+4,iR-(iL+3)); 
				ss >> new_gp;
				gp.push_back(new_gp);
			}
		}
	}

	fin.close();

	// MODIFY TO INFINITE LOOP ASKING FOR PERSON,
	// FACULTY OR STUDENT MODE FOR SORTING
	
	vector <faculty *> faculty_list;
	vector <student *> student_list;
	
	// create seperate vectors of faculty and students
	sptrsort(person_list);
	for (int i = 0; i < (int)person_list.size(); i++) {
		if (person_list[i]->get_ptype() == FACULTY)
			faculty_list.push_back(dynamic_cast<faculty *>(person_list[i]));
		if (person_list[i]->get_ptype() == STUDENT)
			student_list.push_back(dynamic_cast<student *>(person_list[i]));
	}

		

	string mode;
	sptrsort(faculty_list);
	sptrsort(student_list);
	
	printf("command: person\ncommand: faculty\ncommand: student\n\n");

	// continously take input and show information
	while (cin >> mode) {
		printf("command> ");
		if (mode == "person") {	
			for (int i = 0; i < (int)person_list.size(); i++)
				cout << *person_list[i] << "\n";
		}
		if (mode == "faculty") {
			for (int i = 0; i < (int)faculty_list.size(); i++) 
				cout << *faculty_list[i] << "\n";
		}
		if (mode == "student") {
			for (int i = 0; i < (int)student_list.size(); i++)
				cout << *student_list[i] << "\n";
		}
	}
	printf("command> \n");
// RELEASE DYNAMICALLY ALLOACTED MEMOY
	for (int i = 0; i < (int)person_list.size(); i++)
		delete person_list[i];
}
