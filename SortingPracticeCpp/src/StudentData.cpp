/*
 * StudentData.cpp
 *
 *  Created on: Apr 14, 2025
 *      Author: joe
 */

//#include <cstdlib>
//#include <ctime>

//#include <iostream>
//#include <iomanip>
#include "StudentData.h"

#include <cstring>

using namespace std;

/*			string array representing Student_Major			*/
#undef _STUDENT_MAJOR
#define _STUDENT_MAJOR(mjr)	#mjr,
std::string student_major_strings[] = { _STUDENT_MAJORS };


/*	***************************************************************	*/
/*	***************************************************************	*/
/*							member functions						*/
/*	***************************************************************	*/
/*	***************************************************************	*/

int StudentData::compareTo(const StudentData &other) const {

	if (this == &other)
		return 0;

//	cout << "in compareTo(StudentData&) " << this->last_name << " vs " << other.last_name << " ";
	int string_compare_result;
	string_compare_result = last_name.compare(other.last_name);
//	string_compare_result = strcmp(last_name.c_str(), other->last_name.c_str());
	if (string_compare_result > 0)	return 1;
	if (string_compare_result < 0)	return -1;
	//	last_name == other.last_name
//	cout << ", " << this->first_name << " vs " << other.first_name << " ";
	string_compare_result = first_name.compare(other.first_name);
//	string_compare_result = strcmp(first_name.c_str(), other->first_name.c_str());
	if (string_compare_result > 0)	return 1;
	if (string_compare_result < 0)	return -1;

	// last_name == other.last_name &&
	// first_name == other.first_name
	if (!isValid(other.major)) {
		return 0;
	}

	if (major > other.major) {
		return 1;
	}
	if (major < other.major) {
		return -1;
	}
	// don't compare GPAs
	return 0;
}

// the two fundamental relational operators

bool StudentData::operator==(const StudentData &other) const {
	return this->compareTo(other) == 0;
}

bool StudentData::operator<(const StudentData &other) const {
	return this->compareTo(other) < 0;
}

//	the derived relational operators

bool StudentData::operator!=(const StudentData &other) const {
	return !(*this == other);
}

bool StudentData::operator>(const StudentData &other) const	{
	return !((*this == other) || (*this < other));
}

bool StudentData::operator>=(const StudentData &other) const	{
	return !(*this < other);
}

bool StudentData::operator<=(const StudentData &other) const {
	return (*this == other) || (*this < other);
}

std::string		StudentData::lastName(void) const		{ 	return last_name;	}
std::string		StudentData::firstName(void) const		{ 	return first_name;	}
StudentMajor	StudentData::studentMajor(void) const	{	return major;	}
double			StudentData::GPA(void) const			{	return gpa;		}

StudentData&	StudentData::lastName(const std::string &ln) {
	last_name = ln;
	return *this;
}

StudentData&	StudentData::firstName(const std::string &fn) {
	first_name = fn;
	return *this;
}

StudentData&	StudentData::studentMajor(StudentMajor mjr) {
	major = mjr;
	return *this;
}

StudentData&	StudentData::GPA(double _gpa) {
	gpa = _gpa;
	return *this;
}

std::string StudentData::str() {
	//	prevent 'left' 'fixed', etc from propagting outside this function
	std::ios_base::fmtflags _flags = std::cout.flags();
	stringstream ss;
	ss	<< setw(STUDENT_DATA_LAST_NAME_WIDTH) << left << last_name
		<< setw(STUDENT_DATA_FIRST_NAME_WIDTH) << left << first_name
		<< setw(STUDENT_DATA_MAJOR_WIDTH) <<  left << major
		<< setw(STUDENT_DATA_GPA_WIDTH) << fixed << setprecision(2) << gpa;
	cout.flags(_flags);
	return ss.str();
}

std::ostream& operator<<(std::ostream& out, StudentData &object) {
	out << object.str();
	return out;
}

/*	this is marked private so that the user cannot create an uninitialized object */

StudentData::StudentData() {
	last_name = "";
	first_name = "";
	major = StudentMajor::Undeclared_Major;
	gpa = 0.0;
}

StudentData::~StudentData() {
}

StudentData::StudentData(const StudentData &other) {

	if (this == &other)
		return;

	last_name = other.last_name;
	first_name = other.first_name;
	major = other.major;
	gpa = other.gpa;
}

StudentData::StudentData(StudentData &&other) noexcept {

	if (this == &other)
		return;

	last_name = other.last_name;
	first_name = other.first_name;
	major = other.major;
	gpa = other.gpa;
}

StudentData::StudentData(const std::string last_name, const std::string first_name,  StudentMajor major, double gpa) :
	last_name(last_name), first_name(first_name), major(major), gpa(gpa) { }

StudentData& StudentData::operator=(const StudentData &other) {

	if (this != &other) {
		last_name = other.last_name;
		first_name = other.first_name;
		major = other.major;
		gpa = other.gpa;
	}

	return *this;
}

StudentData& StudentData::operator=(StudentData &&other) noexcept {

	if (this != &other) {
		last_name = other.last_name;
		first_name = other.first_name;
		major = other.major;
		gpa = other.gpa;
	}

	return *this;
}


void initArraySequential(StudentData **list, int num_elements) {

}

void initArrayFewDistinct(StudentData **list, int num_elements, int num_distinct) {

}

void initializeArray(StudentData **, StudentData*, int num_elements) {

}


/*	******************************************************	*/
/*	******************************************************	*/
/*			Student_Major objects & functions				*/
/*	******************************************************	*/
/*	******************************************************	*/

const int invalidStudentMajor	= static_cast<int>(INVALID_STUDENT_MAJOR);
const int minStudentMajor		= static_cast<int>(MIN_STUDENT_MAJOR);
const int maxStudentMajor		= invalidStudentMajor-1;

bool isValid(StudentMajor major) {

	if (static_cast<int>(major) > maxStudentMajor)	return false;
	if (static_cast<int>(major) < minStudentMajor)	return false;
	return true;
}

std::string convertToString(StudentMajor major) {


	if (!isValid(major)) {
		std::stringstream retval;
		retval 	<< "ERROR: toString(" << major << " is not in valid range ["
				<< minStudentMajor << ":" << maxStudentMajor << "]";
		return retval.str();
	} else {
		return student_major_strings[static_cast<int>(major)];
	}
}

/*		std::ostream operator<< overload					*/

std::ostream& operator<<(std::ostream& out, StudentMajor major) {

	out << convertToString(major);
	return out;
}


/*	**************************************************************	*/
/*							test fixture							*/
/*	**************************************************************	*/

static const unsigned	EQ	= 0x01;
static const unsigned	NE	= 0x02;
static const unsigned	GT	= 0x04;
static const unsigned	GE	= 0x08;
static const unsigned	LT	= 0x10;
static const unsigned	LE 	= 0x20;

unsigned preformRelationalOperators(StudentData &u, StudentData &v) {

	unsigned returned_flags = 0;

	if (u == v)	returned_flags 	|= EQ;
	if (u != v)	returned_flags 	|= NE;
	if (u > v)	returned_flags 	|= GT;
	if (u >= v)	returned_flags	|= GE;
	if (u < v)	returned_flags	|= LT;
	if (u <= v)	returned_flags	|= LE;

	return returned_flags;
}

bool testRel(StudentData &u, StudentData &v, unsigned expected) {

	unsigned result = preformRelationalOperators(u, v);
	unsigned failed_tests = result ^ expected;
	bool all_tests_passed = true;

	if (failed_tests & EQ) {
		all_tests_passed = false;
		if (expected & EQ) {
			cout << "failed to return true  on:  " << u << " == " << v << endl;
		} else {
			cout << "failed to return false on:  " << u << " == " << v << endl;
		}
	}
	if (failed_tests & NE) {
		all_tests_passed = false;
		if (expected & NE) {
			cout << "failed to return true  on:  " << u << " != " << v << endl;
		} else {
			cout << "failed to return false on:  " << u << " != " << v << endl;
		}
	}
	if (failed_tests & GT) {
		all_tests_passed = false;
		if (expected & GT) {
			cout << "failed to return true  on:  " << u << " >  " << v << endl;
		} else {
			cout << "failed to return false on:  " << u << " >  " << v << endl;
		}
	}
	if (failed_tests & GE) {
		all_tests_passed = false;
		if (expected & GE) {
			cout << "failed to return true  on:  " << u << " >= " << v << endl;
		} else {
			cout << "failed to return false on:  " << u << " >= " << v << endl;
		}
	}
	if (failed_tests & LT) {
		all_tests_passed = false;
		if (expected & LT) {
			cout << "failed to return true  on:  " << u << " <  " << v << endl;
		} else {
			cout << "failed to return false on:  " << u << " <  " << v << endl;
		}
	}
	if (failed_tests & LE) {
		all_tests_passed = false;
		if (expected & LE) {
			cout << "failed to return true  on:  " << u << " <= " << v << endl;
		} else {
			cout << "failed to return false on:  " << u << " <= " << v << endl;
		}
	}

	if (all_tests_passed) {
			cout << "all tests passed            " << u << " <=> " << v << endl;
	}

	return all_tests_passed;
}


bool testStudentData() {

	cout << "testStudentData()" << endl;

	static const bool testRegularConstructor = false;
	static const bool testCopyConstructor = false;
	static const bool testCopyAssignmentOperator = false;
	static const bool testChainedSetters = false;
	static const bool testGetters = false;
	static const bool testRelationalOperators = false;

	bool passed = true;


	if (testRegularConstructor) {
		StudentData student1("Baker", "Joseph", StudentMajor::Electrical_Engineering, 4.0);
		cout << "student1 &" << &student1 << " : " << student1 << endl;
	}

	if (testCopyConstructor) {
		StudentData student1("Baker", "Joseph", StudentMajor::Electrical_Engineering, 4.0);
		StudentData student2(student1);
		cout << "original student1 &" << &student1 << " : " << student1 << endl;
		cout << "copied student1   &" << &student2 << " : " << student2 << endl;
	}

	if (testCopyAssignmentOperator){
		StudentData student1("Baker", "Joseph", StudentMajor::Electrical_Engineering, 4.0);
		StudentData student2("Cross", "Joseph", StudentMajor::Electrical_Engineering, 4.0);
		cout << "student1 &" << &student1 << " : " << student1 << endl;
		cout << "student2 &" << &student2 << " : " << student2 << endl;
		cout << "after copy assignment: student2 = student1;" << endl;
		student2 = student1;
		cout << "student1 &" << &student1 << " : " << student1 << endl;
		cout << "student2 &" << &student2 << " : " << student2 << endl;
	}

	if (testChainedSetters){
		StudentData student1("Baker", "Joseph", StudentMajor::Electrical_Engineering, 4.0);
		cout << "student1 &" << &student1 << " : " << student1 << endl;
		cout << "after chained assignment student1.lastName().firstName().studenMajor().GPA():" << endl;
		student1.lastName("Clown").firstName("Bozo").studentMajor(StudentMajor::Industrial_Management).GPA(-1.0);
	}

	if (testGetters) {
		StudentData student1("Baker", "Joseph", StudentMajor::Electrical_Engineering, 4.0);
		cout << "student1 &" << &student1 << " : " << student1 << endl;
		cout << "student1.lastName() = " << student1.lastName() << endl;
		cout << "student1.firstName() = " << student1.firstName() << endl;
		cout << "student1.studentMajor() = " << student1.studentMajor() << endl;
		cout << "student1.GPA() = " << student1.GPA() << endl;
	}

	StudentData student0("Baker", "Joseph", StudentMajor::Electrical_Engineering, 4.0);
	StudentData student4("Cross", "Susan" , StudentMajor::Mechanical_Engineering, 2.5);
	StudentData student1("Cross", "Joseph", StudentMajor::Electrical_Engineering, 4.0);
	StudentData student3("Cross", "Susan" , StudentMajor::Mechanical_Engineering, 4.0);
	StudentData student2("Cross", "Susan" , StudentMajor::Electrical_Engineering, 4.0);

	if (testRelationalOperators) {
		testRel(student0, student0, EQ | GE | LE);
		testRel(student0, student1, NE | LT | LE);	// last name differs
		testRel(student1, student0, NE | GT | GE);
		testRel(student1, student2, NE | LT | LE);	// first name differs
		testRel(student2, student1, NE | GT | GE);
		testRel(student2, student3, NE | GT | GE);	// majore differs
		testRel(student3, student2, NE | LT | LE);
		testRel(student3, student4, EQ | GE | LE);	// GPA differs
		testRel(student4, student3, EQ | GE | LE);
	}

	cout << "testStudentData() returns " << (passed ? "PASSED" : "FAILED") << endl;
	return passed;
}

