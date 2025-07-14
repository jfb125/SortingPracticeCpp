/*
 * Build_Test_Array.h
 *
 *  Created on: Apr 15, 2025
 *      Author: joe
 */

#ifndef STUDENTDATAGENERATOR_H_
#define STUDENTDATAGENERATOR_H_

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "SortTest.h"
/*	for struct Sortable_Object	*/
#include "StudentData.h"

//	Enable_Carry_In causes the field to be updated when the previous
//	  	field has rolled over IF and only IFF both fields have
//		advance mode set to Sequential
//		i.e. last_name "aaaa", first_name "zzzz" becomes "aaab", "aaaa"
//	If a fields update is set to None, the result of the previous
//		field does not matter: the field will not be updated
//	If a fields update is set to random, the field will get updated
//		regardless of the previous field's update mode

enum class Advance_Mode {
	None,
	Random,
	Sequential_Disabled_Carry_In,
	Sequential_Enabled_Carry_In
};

bool isValid(Advance_Mode mode);
std::ostream& operator<<(std::ostream& out, Advance_Mode mode);
std::string convertToString(Advance_Mode);


class StudentDataGenerator {

private:
	std::string	 initial_last_name;
	std::string  initial_first_name;
	StudentMajor initial_student_major;
	double		 initial_student_GPA;

	std::string  current_last_name;
	std::string  current_first_name;
	StudentMajor current_student_major;
	double 		 current_student_GPA;

	Advance_Mode last_name_advance_mode;
	Advance_Mode first_name_advance_mode;
	Advance_Mode student_major_advance_mode;
	bool 		 is_initialized;

public:

	// individual set methods

	StudentDataGenerator& lastNameAdvanceMode(Advance_Mode);
	StudentDataGenerator& firstNameAdvanceMode(Advance_Mode);
	StudentDataGenerator& studentMajorAdvanceMode(Advance_Mode);
	StudentDataGenerator& isInitialized(bool);
	StudentDataGenerator& initialLastName(const std::string&);
	StudentDataGenerator& initialFirstName(const std::string&);
	StudentDataGenerator& initialStudentMajor(StudentMajor);
	StudentDataGenerator& initialGPA(double);
	StudentDataGenerator& currentLastName(const std::string&);
	StudentDataGenerator& currentFirstName(const std::string&);
	StudentDataGenerator& currentStudentMajor(StudentMajor);
	StudentDataGenerator& currentGPA(double);
	StudentDataGenerator& isInitialize(bool);

	// individual get methods

	std::string	 initialLastName(void) const;
	std::string	 initialFirstName(void) const;
	StudentMajor initialStudentMajor(void) const;
	double		 initialGPA(void) const;

	std::string	 currentLastName(void) const;
	std::string	 currentFirstName(void) const;
	StudentMajor currentStudentMajor(void) const;
	double		 currentGPA(void) const;

	Advance_Mode lastNameAdvanceMode(void) const;
	Advance_Mode firstNameAdvanceMode(void) const;
	Advance_Mode studentMajorAdvanceMode(void) const;

	bool 		 isInitialized(void) const;


	//	mutators

	void		 reset(void);	// copies 'initial' into 'current'

	//	this returns a pointer to a StudentData type in which
	//	  the variables have been set to 'current' values
	//	and then updates the 'current' values to the next student record
    StudentData* getNextStudent();	// r

    // 	formatted output
    std::string  toString() const;

    // constructors, destructor and assignment operators
    // initializes fields, but assigns 'is_initialized' = false
    StudentDataGenerator();
	~StudentDataGenerator() {}

	StudentDataGenerator(const StudentDataGenerator&);
	StudentDataGenerator& operator=(const StudentDataGenerator&);

	StudentDataGenerator(	std::string lname,
					std::string fname,
					StudentMajor maj,
					double avg,
					Advance_Mode lname_adv,
					Advance_Mode fname_adv,
					Advance_Mode mgr_adv,
					bool isInit) :
						initial_last_name(lname),
						initial_first_name(fname),
						initial_student_major(maj),
						initial_student_GPA(avg),
						last_name_advance_mode(lname_adv),
						first_name_advance_mode(fname_adv),
						student_major_advance_mode(mgr_adv),
						is_initialized(isInit) {
		current_last_name 		= initial_last_name;
		current_first_name 		= initial_first_name;
		current_student_major	= initial_student_major;
		current_student_GPA		= initial_student_GPA;
	}
};

std::ostream& operator<<(std::ostream& out, const StudentDataGenerator&);

//  be able to generate 100,000,000 distinct values
#define _Next_Student_initial_is_initialized false
#define _Next_Student_initial_last_name     "aaaaaaaaaa"
#define _Next_Student_initial_first_name    "aaaaaaaaaa"
#define _Next_Student_initial_major         StudentMajor::Undeclared_Major
#define _Next_Student_initial_gpa			0.0
#define _Next_Student_initial_last_name_advance_mode		Advance_Mode::None
#define _Next_Student_initial_first_name_advance_mode		Advance_Mode::None
#define _Next_Student_initial_student_major_advance_mode	Advance_Mode::None

#endif /* NEXT_STUDENT_H_ */
