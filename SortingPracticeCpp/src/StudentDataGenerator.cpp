/*
 * Next_Student.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: joe
 */
#include "StudentDataGenerator.h"

using namespace std;

/* ************************************************************	*/
/*		functions dealing with Advance_Mode enum class			*/
/* ************************************************************	*/

bool isValid(Advance_Mode mode) {
	switch(mode) {
	case Advance_Mode::None:							return true;
	case Advance_Mode::Random:							return true;
	case Advance_Mode::Sequential_Disabled_Carry_In:	return true;
	case Advance_Mode::Sequential_Enabled_Carry_In:		return true;
	default:											return false;
	}
}

std::string convertToString(Advance_Mode mode) {
	switch(mode) {
	//															0         1         2
	//															0123456789012345678901234567
	case Advance_Mode::Sequential_Disabled_Carry_In:	return "Sequential Disabled Carry In";
	case Advance_Mode::Sequential_Enabled_Carry_In:		return "Sequential Enabled carry In";
	case Advance_Mode::Random:							return "Randomly";
	case Advance_Mode::None:							return "None";
	default:											return "Invalid Advance_Mode";
	}
}

const int advance_mode_string_width = 27;

std::ostream& operator<<(std::ostream& out, Advance_Mode mode) {
	if (isValid(mode))
		out << convertToString(mode);
	else
		out << "advance mode not valid";
	return out;
}


/* ************************************************************	*/
/* ************************************************************	*/
/*					class Student_Data_Generator				*/
/* ************************************************************	*/
/* ************************************************************	*/

bool isValidGPA(double GPA)	{
	return (!(GPA<0.0) && !(GPA>4.0));
}

/*	******************************************************	*/
/*					class declaration						*/
/*	******************************************************	*/

/*	******************************************************	*/
/*		constructors / destructor / copy assignment			*/
/*	******************************************************	*/

StudentDataGenerator::StudentDataGenerator() {

	initial_last_name			= _Next_Student_initial_last_name;
	initial_first_name			= _Next_Student_initial_first_name;
	initial_student_major		= _Next_Student_initial_major;
	initial_student_GPA			= _Next_Student_initial_gpa;

	last_name_advance_mode		= _Next_Student_initial_last_name_advance_mode;
	first_name_advance_mode		= _Next_Student_initial_first_name_advance_mode;
	student_major_advance_mode	= _Next_Student_initial_student_major_advance_mode;

	current_last_name			= initial_last_name;
	current_first_name			= initial_first_name;
	current_student_major		= initial_student_major;
	current_student_GPA			= initial_student_GPA;

	is_initialized				= _Next_Student_initial_is_initialized;
}

StudentDataGenerator::StudentDataGenerator(const StudentDataGenerator& other) {

	initial_last_name 		= other.initial_last_name;
	initial_first_name		= other.initial_first_name;
	if (isValid(other.initial_student_major)) {
		initial_student_major	= other.initial_student_major;
	} else {
		initial_student_major	= _Next_Student_initial_major;
	}
	if (isValidGPA(other.initial_student_GPA)) {
		initial_student_GPA		= other.initial_student_GPA;
	} else {
		initial_student_GPA = _Next_Student_initial_gpa;
	}

	current_last_name 		= other.current_last_name;
	current_first_name		= other.current_first_name;
	if (isValid(other.current_student_major)) {
		current_student_major	= other.current_student_major;
	} else {
		current_student_major	= _Next_Student_initial_major;
	}
	if (isValidGPA(other.current_student_GPA)) {
		current_student_GPA		= other.current_student_GPA;
	} else {
		initial_student_GPA = _Next_Student_initial_gpa;
	}

	if (isValid(other.last_name_advance_mode)) {
		last_name_advance_mode		= other.last_name_advance_mode;
	} else {
		last_name_advance_mode		= Advance_Mode::None;
	}
	if (isValid(other.first_name_advance_mode)) {
		first_name_advance_mode		= other.first_name_advance_mode;
	} else {
		first_name_advance_mode		= Advance_Mode::None;
	}
	if (isValid(other.student_major_advance_mode)) {
		student_major_advance_mode	= other.student_major_advance_mode;
	} else {
		student_major_advance_mode 	= Advance_Mode::None;
	}
	is_initialized = other.is_initialized;
}

StudentDataGenerator& StudentDataGenerator::operator=(const StudentDataGenerator& other) {

	if (&other == this)
		return *this;

	StudentDataGenerator *retval = new StudentDataGenerator;

	retval->initial_last_name	= other.initial_last_name;
	retval->initial_first_name	= other.initial_first_name;
	if (isValid(other.initial_student_major)) {
		retval->initial_student_major = other.initial_student_major;
	} else {
		retval->initial_student_major = _Next_Student_initial_major;
	}
	if (isValidGPA(other.initial_student_GPA)) {
		retval->initial_student_GPA = other.initial_student_GPA;
	} else {
		retval->initial_student_GPA = _Next_Student_initial_gpa;
	}

	retval->current_last_name	= other.current_last_name;
	retval->current_first_name	= other.current_first_name;
	if (isValid(other.current_student_major)) {
		retval->current_student_major = other.current_student_major;
	} else {
		retval->current_student_major = _Next_Student_initial_major;
	}
	if (isValidGPA(other.current_student_GPA)) {
		retval->current_student_GPA = other.current_student_GPA;
	} else {
		retval->current_student_GPA = _Next_Student_initial_gpa;
	}

	if (isValid(other.last_name_advance_mode)) {
		retval->last_name_advance_mode = other.last_name_advance_mode;
	} else {
		retval->last_name_advance_mode = Advance_Mode::None;
	}

	if (isValid(other.first_name_advance_mode)) {
		retval->first_name_advance_mode = other.first_name_advance_mode;
	} else {
		retval->first_name_advance_mode = Advance_Mode::None;
	}

	if (isValid(other.student_major_advance_mode)) {
		retval->student_major_advance_mode = other.student_major_advance_mode;
	} else {
		retval->student_major_advance_mode = Advance_Mode::None;
	}

	retval->is_initialized = other.is_initialized;

	return *this;
}

/* ****************************************************************	*/
/*						individual set methods						*/
/* ****************************************************************	*/

StudentDataGenerator& StudentDataGenerator::lastNameAdvanceMode(Advance_Mode mode) {
	if (isValid(mode)) { last_name_advance_mode = mode; }
	return *this;
}

StudentDataGenerator& StudentDataGenerator::firstNameAdvanceMode(Advance_Mode mode) {
	if (isValid(mode)) { first_name_advance_mode = mode; }
	return *this;
}

StudentDataGenerator& StudentDataGenerator::studentMajorAdvanceMode(Advance_Mode mode) {
	if (isValid(mode)) { student_major_advance_mode = mode; }
	return *this;
}

		/*	initial values	*/

StudentDataGenerator& StudentDataGenerator::initialLastName(const std::string &name) {
	initial_last_name = name; return *this;
}

StudentDataGenerator& StudentDataGenerator::initialFirstName(const std::string &name) {
	initial_first_name = name;
	return *this;
}

StudentDataGenerator& StudentDataGenerator::initialStudentMajor(StudentMajor major) {
	if (isValid(major))
		initial_student_major = major;
	else
		initial_student_major = _Next_Student_initial_major;
	return *this;
}

StudentDataGenerator& StudentDataGenerator::initialGPA(double avg) {
	if (isValidGPA(avg))
		initial_student_GPA = avg;
	else
		initial_student_GPA = _Next_Student_initial_gpa;
	return *this;
}

		/*		current values		*/

StudentDataGenerator& StudentDataGenerator::currentLastName(const std::string &name) {
	current_last_name = name;
	return *this;
}

StudentDataGenerator& StudentDataGenerator::currentFirstName(const std::string &name) {
	current_first_name = name;
	return *this;
}

StudentDataGenerator& StudentDataGenerator::currentStudentMajor(StudentMajor major) {
	if (isValid(major))
		current_student_major = major;
	else
		current_student_major = _Next_Student_initial_major;
	return *this;
}

StudentDataGenerator& StudentDataGenerator::currentGPA(double avg) {
	if (isValidGPA(avg))
		initial_student_GPA = avg;
	else
		initial_student_GPA = _Next_Student_initial_gpa;
	return *this;
}

StudentDataGenerator& StudentDataGenerator::isInitialized(bool is_it) {
	is_initialized = is_it;
	return *this;
}

void	StudentDataGenerator::reset(void) {

	current_last_name 		= initial_last_name;
	current_first_name		= initial_first_name;
	if (isValid(initial_student_major)) {
		current_student_major	= initial_student_major;
	} else {
		current_student_major	= _Next_Student_initial_major;
	}
	if (isValidGPA(initial_student_GPA)) {
		current_student_GPA		= initial_student_GPA;
	} else {
		current_student_GPA		= _Next_Student_initial_gpa;
	}
}

/* ****************************************************************	*/
/*						individual get methods						*/
/* ****************************************************************	*/

Advance_Mode StudentDataGenerator::lastNameAdvanceMode(void) const {
	return last_name_advance_mode;
}
Advance_Mode StudentDataGenerator::firstNameAdvanceMode(void) const {
	return first_name_advance_mode;
}
Advance_Mode StudentDataGenerator::studentMajorAdvanceMode(void) const {
	return student_major_advance_mode;
}
bool 		 StudentDataGenerator::isInitialized(void) const {
	return is_initialized;
}

std::string	 StudentDataGenerator::initialLastName(void) const {
	return initial_last_name;
}
std::string	 StudentDataGenerator::initialFirstName(void) const {
	return initial_first_name;
}
StudentMajor StudentDataGenerator::initialStudentMajor(void) const {
	return initial_student_major;
}
double		 StudentDataGenerator::initialGPA() const {
	return initial_student_GPA;
}

std::string	 StudentDataGenerator::currentLastName(void) const {
	return current_last_name;
}
std::string	 StudentDataGenerator::currentFirstName(void) const {
	return current_first_name;
}
StudentMajor StudentDataGenerator::currentStudentMajor(void) const {
	return current_student_major;
}
double		 StudentDataGenerator::currentGPA() const {
	return current_student_GPA;
}

/* ****************************************************************	*/
/*						formatted output							*/
/* ****************************************************************	*/

std::string StudentDataGenerator::toString() const {

	std::stringstream result;
	// construct a StudentData generator because the operator<<(StudentData)
	//	will format the StudentData portions of the _Generator class
	result << "current: " <<  StudentData(current_last_name, current_first_name, current_student_major, current_student_GPA).str() << endl;
	result << "initial: " <<  StudentData(initial_last_name, initial_first_name, initial_student_major, initial_student_GPA).str() << endl;
	result << "last name advance mode:     " << last_name_advance_mode << std::endl;
	result << "first name advance mode:    " << first_name_advance_mode << std::endl;
	result << "student major advance mode: " << student_major_advance_mode << endl;
	result << "generator " << (is_initialized ? " is initialized" : "is NOT initialized");
	return result.str();
}

std::ostream& operator<<(std::ostream& out, const StudentDataGenerator& generator) {

	out << generator.toString();
	return out;
}

/* ****************************************************************	*/
/*						building the next student record			*/
/* ****************************************************************	*/

bool advanceNameSequentially(std::string& existing_value);
void advanceNameRandomly(std::string& existing_value);
bool advanceStudentMajorSequentially(StudentMajor& existing_value);
void advanceStudentMajorRandomly(StudentMajor& existing_value);

StudentData* StudentDataGenerator::getNextStudent() {

	// create the student data that will be returned
	StudentData *retval = new StudentData("", "",
										  _Next_Student_initial_major,
										  _Next_Student_initial_gpa);

	if (!is_initialized) {
		return retval;
	}

	// store the returned value
	retval->last_name 	= current_last_name;
	retval->first_name 	= current_first_name;
	retval->major 		= current_student_major;
	retval->gpa 		= current_student_GPA;

	// update the generator's internal values to the next student
	bool student_major_advance_rolled_over = false;
	bool first_name_advance_rolled_over = false;
	// included here for future possible use
	__attribute__((unused))\
		bool last_name_advance_rolled_over = false;		// unused


	// student_major is the least ordinal of the three fields
	//	thus there is no concept of 'carry in'
	switch (student_major_advance_mode) {
	case Advance_Mode::Sequential_Disabled_Carry_In:
	case Advance_Mode::Sequential_Enabled_Carry_In:
		student_major_advance_rolled_over =
				advanceStudentMajorSequentially(current_student_major);
		break;
	case Advance_Mode::Random:
		advanceStudentMajorRandomly(current_student_major);
		break;
	default:
	case Advance_Mode::None:
		break;
	}

	switch(first_name_advance_mode) {
	case Advance_Mode::Random:
		advanceNameRandomly(current_first_name);
		break;
	case Advance_Mode::Sequential_Disabled_Carry_In:
		first_name_advance_rolled_over =
				advanceNameSequentially(current_first_name);
		break;
	case Advance_Mode::Sequential_Enabled_Carry_In:
		if (student_major_advance_rolled_over)
			first_name_advance_rolled_over =
					advanceNameSequentially(current_first_name);
		break;
	case Advance_Mode::None:
	default:
		break;
	}

	switch(last_name_advance_mode) {
	case Advance_Mode::Random:
		advanceNameRandomly(current_last_name);
		break;
	case Advance_Mode::Sequential_Disabled_Carry_In:
		last_name_advance_rolled_over =
				advanceNameSequentially(current_last_name);
		break;
	case Advance_Mode::Sequential_Enabled_Carry_In:
		if (first_name_advance_rolled_over)
			last_name_advance_rolled_over =
					advanceNameSequentially(current_last_name);
		break;
	case Advance_Mode::None:
	default:
		break;
	}

	return retval;
}

/*
 * 	figure out the length of the string
 * 	moving right to left, 	if a char is 'z' -> 'a' and carry
 * 							if a char is 'Z' -> 'Z' and carry
 * 	if the left most char generates a carry, return true
 * 	otherwise return false
 */

bool advanceNameSequentially(std::string& existing_value) {

	int current_char = existing_value.length()-1;
	bool carry_occured = false;

//	cout << "before: " << existing_value << endl;
	while(current_char >= 0) {
		carry_occured = false;
		switch(existing_value[current_char]) {
		case 'Z' :	existing_value[current_char] = 'A';
					carry_occured = true;
					break;
		case 'z' :	existing_value[current_char] = 'a';
					carry_occured = true;
					break;
		default:	existing_value[current_char]++;
					break;
		}
		current_char--;
		if (!carry_occured)
			break;
	}

//	cout << "after:  " << existing_value << endl;
	return carry_occured;
}

void advanceNameRandomly(std::string& existing_value) {

	static bool isRandomInitialized = false;

	// if the random number generator is not initialized, do so
	if (!isRandomInitialized) {
		std::srand(std::time(0));
		isRandomInitialized = true;
	}

	int existing_length = existing_value.length();

	//	if the string is empty, no need to modify it
	if (existing_length == 0)
		return;

	int char_to_modify = std::rand() % existing_length;

	constexpr int char_span = 'z' - 'a' + 1;	// 26
	constexpr int digit_span = '9' - '0' + 1;	// 10

	if (isupper(existing_value[char_to_modify])) {
		int intval = existing_value[char_to_modify] - 'A';
		intval += std::rand();
		intval %= char_span;
		existing_value[char_to_modify] = intval + 'A';
	} else
	if (islower(existing_value[char_to_modify])) {
		int intval = existing_value[char_to_modify] - 'a';
		intval += std::rand();
		intval %= char_span;
		existing_value[char_to_modify] = intval + 'a';
	} else
	if (isdigit(existing_value[char_to_modify])) {
		int intval = existing_value[char_to_modify] - '0';
		intval += std::rand();
		intval %= digit_span;
		existing_value[char_to_modify] = intval + '0';
	} else {
		existing_value[char_to_modify] =
			(std::rand() % char_span) + 'A';
	}
}

bool advanceStudentMajorSequentially(StudentMajor& existing_value) {

	if (!isValid(existing_value))
		return false;

	if (existing_value == MAX_STUDENT_MAJOR) {
		existing_value = MIN_STUDENT_MAJOR;
		return true;	// carry occured
	}

	int int_val = static_cast<int>(existing_value);
	int_val++;
	existing_value = static_cast<StudentMajor>(int_val);
	return false;	// no carry occured
}

void advanceStudentMajorRandomly(StudentMajor& existing_value) {

	static bool isRandomInitialized = false;

	// if the random number generator is not initialized, do so
	if (!isRandomInitialized) {
		std::srand(std::time(0));
		isRandomInitialized = true;
	}

	int student_major_span = static_cast<int>(MAX_STUDENT_MAJOR) -
							 static_cast<int>(MIN_STUDENT_MAJOR) + 1;

	int int_val = static_cast<int>(existing_value);
	int_val += std::rand();
	int_val %= student_major_span;
	existing_value = static_cast<StudentMajor>(existing_value);

	if (!isValid(existing_value)) {
		std::cout << "ERROR: 'advanceStudentMajorRandomly() created invalid StudentMajor" << endl;
	}
}

/* ****************************************************************	*/
/* ****************************************************************	*/
/*						building the array							*/
/* ****************************************************************	*/
/* ****************************************************************	*/

int buildDataArray(StudentData **dst, int num, StudentDataGenerator &generator) {

	if (dst)
		*dst = nullptr;

	if (num < 0)
		return 0;

	dst = new StudentData*[num];
	for (int i = 0; i != num; i++) {
		dst[i]	= generator.getNextStudent();
	}

	return num;
}


void deleteStudentArray(StudentData **src, int num) {

	if (src == nullptr)
		return;

	if (num <= 0) {
		delete src;
		return;
	}

	for (int i = 0; i != num; i++) {

		if (src[i])
			delete src[i];
	}

	delete src;
}
