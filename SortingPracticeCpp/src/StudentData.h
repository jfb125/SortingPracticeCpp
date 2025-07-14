/*
 * StudentData.h
 *
 *  Created on: Apr 14, 2025
 *      Author: joe
 */

#ifndef STUDENTDATA_H_
#define STUDENTDATA_H_

#include <iostream>
#include <iomanip>
#include <string>

#undef _STUDENT_MAJOR
#define _STUDENT_MAJORS \
	_STUDENT_MAJOR(Undeclared_Major)\
	_STUDENT_MAJOR(Mechanical_Engineering)\
	_STUDENT_MAJOR(Industrial_Engineering)\
	_STUDENT_MAJOR(Nuclear_Engineering)\
	_STUDENT_MAJOR(Chemical_Engineering)\
	_STUDENT_MAJOR(Biochemical_Engineering)\
	_STUDENT_MAJOR(Aerospace_Engineering)\
	_STUDENT_MAJOR(Computer_Engineerings)\
	_STUDENT_MAJOR(Architecture)\
	_STUDENT_MAJOR(Industrial_Management)\
	_STUDENT_MAJOR(Electrical_Engineering)\
	_STUDENT_MAJOR(Student_Major_Count)

#define MIN_STUDENT_MAJOR		StudentMajor::Undeclared_Major
#define MAX_STUDENT_MAJOR		StudentMajor::Electrical_Engineering
#define INVALID_STUDENT_MAJOR	StudentMajor::Student_Major_Count

#define _STUDENT_MAJOR(maj)	maj,

enum class StudentMajor {
	_STUDENT_MAJORS
};

#define STUDENT_DATA_LAST_NAME_WIDTH	12
#define STUDENT_DATA_FIRST_NAME_WIDTH	12
#define STUDENT_DATA_MAJOR_WIDTH		25
#define	STUDENT_DATA_GPA_WIDTH			 5
#define	STUDENT_DATA_GPA_PRECISION		 2

std::ostream& operator<<(std::ostream& out, StudentMajor);
std::string convertToString(StudentMajor);
bool isValid(StudentMajor);

class StudentData {
private:

public:
	std::string 	last_name;
	std::string 	first_name;
	StudentMajor	major;
	double 			gpa;

	int compareTo(const StudentData&) const;

	bool operator>(const StudentData&) const;
	bool operator<(const StudentData&) const;
	bool operator<=(const StudentData&) const;
	bool operator>=(const StudentData&) const;
	bool operator==(const StudentData&) const;
	bool operator!=(const StudentData&) const;

	std::string		lastName(void) const;
	StudentData&	lastName(const std::string &);
	std::string		firstName(void) const;
	StudentData&	firstName(const std::string &);
	StudentMajor	studentMajor(void) const;
	StudentData&	studentMajor(StudentMajor);
	double			GPA(void) const;
	StudentData&	GPA(double);

	std::string str();

	StudentData();
	virtual ~StudentData();
	StudentData(const StudentData &other);
	StudentData(StudentData &&other) noexcept;
	StudentData& operator=(const StudentData &other);
	StudentData& operator=(StudentData &&other) noexcept;
	StudentData(const std::string last_name, const std::string first_name,  StudentMajor major = StudentMajor::Undeclared_Major, double gpa = 0.0);
};

std::ostream& operator<<(std::ostream& out, StudentData &obj);

bool testStudentData();

#endif /* STUDENTDATA_H_ */
