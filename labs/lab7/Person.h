#ifndef __PERSON_H__ 
#define __PERSON_H__

//#include WHAT'S NEEDED
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

enum person_enum  { UNKNOWN, FACULTY, STUDENT };
enum faculty_enum { ASST_PROF, ASSOC_PROF, FULL_PROF };
enum student_enum { FRESHMAN, SOPHOMORE, JUNIOR, SENIOR };

// abstract class with children faculty and students
class person {
	
	public:
		person() {}
		virtual ~person() {}
		friend ostream & operator<<(ostream &, const person &);
		bool operator < (const person &p2) const;
		person_enum get_ptype() { return ptype; } 

	protected:
		person_enum ptype;
		string name;
		virtual void print_details() const =0;
		virtual void print_courses() const  =0;

};

//class faculty DEFINITION -- derived class
class faculty : public person {

	public:	

		faculty(string Name, faculty_enum type, vector <string> &Course);
		~faculty() {};
		bool operator < (const faculty &p2) const;

	protected:	
		faculty_enum faculty_type;
		multiset <string> courselist;
		void print_details() const;
		void print_courses() const;
};


//class student DEFINITION -- derived class
class student : public person {
	public:
		student(string Name, student_enum type, vector <string> &Course, vector <float> &gp);
		~student() {};
		bool operator < (const student &p2) const;

	protected:		
		student_enum student_type;
		multimap <string, float> courselist;
		void print_details() const;
		void print_courses() const;
};
#endif
