/*
 * SortAlgorithm.cpp
 *
 *  Created on: Apr 16, 2025
 *      Author: joe
 */

#include "SortAlgorithm.h"

/*	***********************************************************	*/
/*				string representation of enum values			*/
/*	***********************************************************	*/

#undef _SORT_ALGORITHM
#define _SORT_ALGORITHM(alg)	#alg,
std::string sort_algorithm_strings[] = { _SORT_ALGORITHMS };

/*	***********************************************************	*/
/*						constants								*/
/*	***********************************************************	*/

const int min_sort_algorithm = static_cast<int>(MIN_SORT_ALGORITHM);
const int max_sort_algorithm = static_cast<int>(MAX_SORT_ALGORITHM);
const int invalid_sort_algorithm = static_cast<int>(INVALID_SORT_ALGORITHM);

/* ************************************************************	*/
/*							safety								*/
/* ************************************************************	*/

bool isValid(SortAlgorithms in_question) {

	int inq = static_cast<int>(in_question);

	if (inq < min_sort_algorithm)	return false;
	if (inq > max_sort_algorithm)	return false;
	return true;
}

bool areValid(SortAlgorithms u, SortAlgorithms v) {
	return isValid(u) && isValid(v);
}

/* ************************************************************	*/
/*						incr/decr								*/
/* ************************************************************	*/

SortAlgorithms& operator++(SortAlgorithms& alg) {

	if (!isValid(alg)) {
		std::cout << "ERROR: SortAlgorithms& operator++(SortAlgorithms&) passed invalid parameter" << std::endl;
		alg = SortAlgorithms::SORT_ALGORITHMS_COUNT;
	} else {
		int alg_int = static_cast<int>(alg);
		alg = static_cast<SortAlgorithms>(++alg_int);
	}
	return alg;
}

SortAlgorithms operator++(SortAlgorithms& alg, int unused) {

	// create a copy to pass back to the caller
	SortAlgorithms retval = alg;

	if (!isValid(alg)) {
		std::cout << "ERROR: SortAlgorithms operator++(SortAlgorithms&, int) passed invalid parameter" << std::endl;
		alg = SortAlgorithms::SORT_ALGORITHMS_COUNT;
	} else {
		++alg;
	}
	return retval;
}

int operator-(SortAlgorithms u, SortAlgorithms v) {

	if (!areValid(u,v)) {
		return 0;
	} else {
		return static_cast<int>(u) - static_cast<int>(v);
	}
}

/* ************************************************************	*/
/*					relational operators						*/
/* ************************************************************	*/

//	required

bool operator==(SortAlgorithms u, SortAlgorithms v) {
	if (areValid(u,v)) {
		return static_cast<int>(u) == static_cast<int>(v);
	} else {
		return false;
	}
}

bool operator<(SortAlgorithms u, SortAlgorithms v) {
	if (areValid(u,v)) {
		return static_cast<int>(u) - static_cast<int>(v) < 0;
	} else {
		return false;
	}
}

//	derived

bool operator!=(SortAlgorithms u, SortAlgorithms v) {
	if (areValid(u,v)) {
		return !(u == v);
	} else {
		return false;
	}
}

bool operator<=(SortAlgorithms u, SortAlgorithms v) {
	if (areValid(u,v)) {
		return u == v || u < v;
	} else {
		return false;
	}
}

bool operator>=(SortAlgorithms u, SortAlgorithms v) {
	if (areValid(u,v)) {
		return !(u < v);
	} else {
		return false;
	}
}

bool operator>(SortAlgorithms u, SortAlgorithms v) {
	if (areValid(u,v)) {
		return !((u < v) || (u == v));
	} else {
		return false;
	}
}

/*	***********************************************************	*/
/*						formatted output						*/
/*	***********************************************************	*/

std::string unrecognizedSortAlgorithm_toString(SortAlgorithms algorithm) {

	std::stringstream retval;
	retval 	<< "SortAlgorithms <int> "
			<< static_cast<int>(algorithm)
			<< " is not between { "
			<< min_sort_algorithm
			<< " to "
			<< max_sort_algorithm
			<< " }";
	return  retval.str();
}

std::string toString(SortAlgorithms algorithm) {

	if (isValid(algorithm)) {
		return sort_algorithm_strings[static_cast<int>(algorithm)];
	} else {
		return unrecognizedSortAlgorithm_toString(algorithm);
	}
}

std::ostream& operator<<(std::ostream& out, SortAlgorithms algorithm) {

	out << toString(algorithm);
	return out;
}
