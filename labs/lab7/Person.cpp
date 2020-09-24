//#include  WHAT'S NEEDED

#include <cstdio>
#include <algorithm>
#include <string>
#include <set>
#include <map>


using namespace std;

#include "Person.h"

//PERSON CLASS MEMBER FUNCTION IMPLEMENTATION

ostream & operator<<(ostream &out, const person &p) {
	
	p.print_details();
	p.print_courses();

	return out;
}

bool person::operator < (const person &p2) const { 
	if (ptype != p2.ptype)
		return ptype < p2.ptype;
	return name < p2.name;
}

bool faculty::operator < (const faculty &p2) const {
	if (faculty_type != p2.faculty_type)
		return faculty_type < p2.faculty_type;
	return name < p2.name;
}

bool student::operator < (const student &p2) const {
	if (student_type != p2.student_type)
		return student_type < p2.student_type;
	return name < p2.name;
}

//FACULTY CLASS MEMBER FUNCTION IMPLEMENTATION
faculty::faculty(string Name, faculty_enum type, vector <string> &Course) {
	ptype = FACULTY;
	name = Name;
	faculty_type = type; 
	for (int i = 0; i < (int)Course.size(); i++) 
		courselist.insert((Course[i]));
}

void faculty::print_details() const {
	printf("%12s%s\n", "Name: ", name.c_str());

	if (faculty_type == ASST_PROF) 
		printf("%12s%s", "Category: ", "Assistant Professor\n");

	else if (faculty_type == ASSOC_PROF)
		printf("%12s%s", "Category: ", "Associate Professor\n");

	else if (faculty_type == FULL_PROF)
		printf("%12s%s", "Category: ", "Full Professor\n");
	else 
		printf("Unknown\n");
}

void faculty::print_courses() const {
	set <string>::const_iterator it;
	for (it = courselist.begin(); it != courselist.end(); it++)
		printf("%12s%s\n", "Course: ", it->c_str());
}

//STUDENT CLASS MEMBER FUNCTION IMPLEMENTATION
student::student(string Name, student_enum type, vector <string> &Course, vector <float> &gp) {
	ptype = STUDENT;
	name = Name;
	student_type = type;
	for (int i = 0; i < (int)Course.size(); i++) 
		courselist.insert(make_pair(Course[i], gp[i]));

}

void student::print_details() const {
	printf("%12s%s\n", "Name: ", name.c_str());
	
	if (student_type == FRESHMAN)
		printf("%12s%s", "Category: ", "Freshman\n");

	else if (student_type == SOPHOMORE)
		printf("%12s%s", "Category: ", "Sophomore\n");

	else if (student_type ==  JUNIOR)
		printf("%12s%s", "Category: ", "Junior\n");

	else if (student_type ==  SENIOR)
		printf("%12s%s", "Category: ", "Senior\n");
	else 
		printf("Unknown\n");
}

void student::print_courses() const {
	float sum = 0;
	float i = 0;
	multimap <string, float>::const_iterator it;
	for (it = courselist.begin(); it != courselist.end(); it++) {
		i++;
		sum += it->second; 
		printf("%12s%-26s%.2f %.2f\n", "Course: ", it->first.c_str(), it->second, sum/i);
	}
	
}
