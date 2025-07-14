/*
 * InitialOrdering.cpp
 *
 *  Created on: Apr 16, 2025
 *      Author: joe
 */

#include "InitialOrdering.h"

/* ***********************************************************************	*/
/* ***********************************************************************	*/
/*						enum InitialOrderings								*/
/* ***********************************************************************	*/
/* ***********************************************************************	*/

/*	**********************************************************************	*/
/*		enum InitialOrderings	representative strings						*/
/*	**********************************************************************	*/

#undef _INITIAL_ORDERING
#define _INITIAL_ORDERING(order)	#order,
std::string initial_ordering_strings[] = { _INITIAL_ORDERINGS };


/*	**********************************************************************	*/
/*		enum InitialOrderings	constants to improve readability			*/
/*	**********************************************************************	*/

constexpr int min_initial_ordering 	= static_cast<int>(MIN_INITIAL_ORDERING);
constexpr int max_initial_ordering 	= static_cast<int>(MAX_INITIAL_ORDERING);
constexpr int num_initial_orderings = static_cast<int>(NUM_INITIAL_ORDERINGS);

/*	**********************************************************************	*/
/*		enum InitialOrderings	functions to validate						*/
/*	**********************************************************************	*/

bool isValid(InitialOrderings tbd) {
	int tbd_int = static_cast<int>(tbd);

	return tbd_int >= min_initial_ordering && tbd_int <= max_initial_ordering;
}

bool areValid(InitialOrderings u, InitialOrderings v) {
	return isValid(u) && isValid(v);
}

bool requiresNumOutOfPlace(InitialOrderings tbd) {

	if (!isValid(tbd))
		return false;

	switch (tbd) {
	case InitialOrderings::FEW_CHANGES:
		return true;
	case InitialOrderings::IN_RANDOM_ORDER:
	case InitialOrderings::NO_CHANGES:
	case InitialOrderings::IN_REVERSE_ORDER:
	default:
		return false;
	}
}

/* ****************************************************************	*/
/*	enum InitialOffertings	arithmetic operators					*/
/* ****************************************************************	*/

InitialOrderings& operator++(InitialOrderings& ordering) {

	if (!isValid(ordering)) {
		std::cout << "passed invalid ordering to 'operator++(InitialOrderings&)" << std::endl;
		return ordering;
	}

	ordering = static_cast<InitialOrderings>(static_cast<int>(ordering) + 1);
	return ordering;
}

InitialOrderings operator++(InitialOrderings& ordering, int unused) {

	InitialOrderings temp = ordering;	// make a copy of previous state

	if (!isValid(ordering)) {
		std::cout << "passed invalid ordering to 'operator++(InitialOrderings, unused)" << std::endl;
	} else {
		++ordering;
	}
	return temp;	// return a value that caller will copy
}

int operator-(InitialOrderings u, InitialOrderings v) {

	if (!areValid(u,v))	return 0;

	return static_cast<int>(u) - static_cast<int>(v);
}

/* ****************************************************************	*/
/*	enum InitialOfferints 	relational operators					*/
/* ****************************************************************	*/

//	fundamental required

bool operator==(InitialOrderings u, InitialOrderings v) {

	if (!areValid(u,v))
		return false;

	return static_cast<int>(u) == static_cast<int>(v);
}

bool operator<(InitialOrderings u, InitialOrderings v) {

	if (!areValid(u,v))
		return false;

	return static_cast<int>(u) < static_cast<int>(v);
}

//	derived

bool operator!=(InitialOrderings u, InitialOrderings v) {

	if (!areValid(u,v))
		return false;

	return !(u == v);
}

bool operator<=(InitialOrderings u, InitialOrderings v) {

	if (!areValid(u,v))
		return false;

	return u < v || u == v;
}

bool operator>(InitialOrderings u, InitialOrderings v) {

	if (!areValid(u,v))
		return false;

	return !(u <= v);
}

bool operator>=(InitialOrderings u, InitialOrderings v) {

	if (!areValid(u,v))
		return false;

	return !(u < v);
}

/*	*******************************************************************	*/
/*	enum InitialOfferings	formatted output							*/
/*	*******************************************************************	*/

std::string unrecognizedInitialOrdering_toString(InitialOrderings order) {

	std::stringstream retval;
	retval 	<< "InitialOrderings<int> "
			<< static_cast<int>(order)
			<< " is not between { "
			<< min_initial_ordering
			<< " to "
			<< max_initial_ordering
			<< " }";
	return retval.str();
}

std::string toString(InitialOrderings order) {

	if (isValid(order)) {
		return initial_ordering_strings[static_cast<int>(order)];
	} else {
		return unrecognizedInitialOrdering_toString(order);
	}
}

std::ostream& operator<<(std::ostream &out, InitialOrderings ordering) {

	out << toString(ordering);
	return out;
}


/* ****************************************************************	*/
/* ****************************************************************	*/
/*						class InitialOrdering						*/
/* ****************************************************************	*/
/* ****************************************************************	*/

/* **************************************************************** */
/*		class InitialOrdering	accessors							*/
/* **************************************************************** */

InitialOrderings InitialOrdering::order(void) const {
	return _ordering;
}

array_size_t InitialOrdering::num_out_of_place(void) const {

	if (_num_out_of_place_is_initialized)
		return _num_out_of_place;
	else
		return DEFAULT_INITIAL_ORDERING_NUM_OUT_OF_PLACE;
}

bool InitialOrdering::num_out_of_place_is_initialized(void) const {
	return _num_out_of_place_is_initialized;
}

/* **************************************************************** */
/*		class InitialOrdering	mutators							*/
/* **************************************************************** */

InitialOrdering& InitialOrdering::order(InitialOrderings ordering) {
	_ordering = ordering;
	return *this;
}

InitialOrdering& InitialOrdering::num_out_of_place(array_size_t num) {
	_num_out_of_place = num;
	return *this;
}

/* **************************************************************** */
/*		class InitialOrdering	relational operators				*/
/* **************************************************************** */

//	required

bool InitialOrdering::operator==(const InitialOrdering &other) const {

	if (this->_ordering != other._ordering)
		return false;
	else
		return true;
}

bool InitialOrdering::operator<(const InitialOrdering &other) const {

	return this->_ordering < other._ordering;
}

//	derived

bool InitialOrdering::operator!=(const InitialOrdering &other) const {

	return !(this->_ordering == other._ordering);
}

bool InitialOrdering::operator <=(const InitialOrdering &other) const {

	return  this->_ordering < other._ordering ||
			this->_ordering == other._ordering;
}

bool InitialOrdering::operator>(const InitialOrdering &other) const {

	return !(this->_ordering <= other._ordering);
}

bool InitialOrdering::operator>=(const InitialOrdering &other) const {

	return !(this->_ordering < other._ordering);
}

/* **************************************************************** */
/*		class InitialOrdering	increment operators					*/
/* **************************************************************** */

InitialOrdering& InitialOrdering::operator++() {

	++this->_ordering;
	return *this;
}

InitialOrdering InitialOrdering::operator++(int unused) {

	InitialOrdering retval(*this);
	++this->_ordering;
	return retval;
}

/* **************************************************************** */
/*		class InitialOrdering	arithmetic operators				*/
/* **************************************************************** */

int InitialOrdering::operator-(const InitialOrdering &other) const {

	return this->_ordering - other._ordering;
}

/* **************************************************************** */
/*		class InitialOrdering	formatted output					*/
/* **************************************************************** */

std::string InitialOrdering::str(void) const {

	std::stringstream retval;
	retval 	<< _ordering;

	if (requiresNumOutOfPlace(_ordering)) {
		retval << " with ";
		if (_num_out_of_place_is_initialized)
			retval << this->_num_out_of_place << " num out of place";
		else
			retval << " num out of place not initialized";
	}
	return retval.str();
}

std::ostream& operator<<(std::ostream &out, const InitialOrdering &object) {

	out << object.str();
	return out;
}

/* **************************************************************** */
/*		class InitialOrdering	c'tors/d'tor/assignment operators	*/
/* **************************************************************** */

InitialOrdering::InitialOrdering() {
	_ordering = DEFAULT_INITIAL_ORDERING_INITIAL_ORDERINGS;
	_num_out_of_place = DEFAULT_INITIAL_ORDERING_NUM_OUT_OF_PLACE;
	_num_out_of_place_is_initialized = DEFAULT_NUM_OUT_OF_PLACE_IS_INITIALIZED;
}

InitialOrdering::~InitialOrdering() {}

InitialOrdering::InitialOrdering(InitialOrderings ordering,
								 array_size_t num_out_of_place) :
										 _ordering(ordering),
										 _num_out_of_place(num_out_of_place) {
	_num_out_of_place_is_initialized = true;
}

InitialOrdering::InitialOrdering(InitialOrderings ordering) {
	_ordering = ordering;
	_num_out_of_place_is_initialized = false;
	_num_out_of_place = DEFAULT_INITIAL_ORDERING_NUM_OUT_OF_PLACE;
}

InitialOrdering::InitialOrdering(const InitialOrdering &other) {
	_ordering = other._ordering;
	_num_out_of_place = other._num_out_of_place;
	_num_out_of_place_is_initialized = true;
}

InitialOrdering& InitialOrdering::operator=(const InitialOrdering &other) {

	if (this != &other) {
		_ordering = other._ordering;
		_num_out_of_place_is_initialized = other._num_out_of_place_is_initialized;
		_num_out_of_place = other._num_out_of_place;
	}
	return *this;
}

//	 does not alter value of '_num_out_of_place'
InitialOrdering& InitialOrdering::operator=(InitialOrderings ordering) {

	_ordering = ordering;
	return *this;
}
