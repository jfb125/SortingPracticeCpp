/*
 * Array_Composition.cpp
 *
 *  Created on: Apr 16, 2025
 *      Author: joe
 */

#include "ArrayComposition.h"

//	string representations

#undef _ARRAY_COMPOSITION
#define _ARRAY_COMPOSITION(composition)	#composition,

std::string array_composition_strings[] = { _ARRAY_COMPOSITIONS };
#undef _ARRAY_COMPOSITION
#undef _ARRAY_COMPOSITIONS

/* ************************************************************	*/
/*							safety								*/
/* ************************************************************	*/

bool isValid(ArrayCompositions in_question) {
	// cast to int in order to bypass the relational operator overloads
	if (static_cast<int>(in_question) < static_cast<int>(MIN_ARRAY_COMPOSITION))
		return false;
	if (static_cast<int>(in_question) > static_cast<int>(MAX_ARRAY_COMPOSITION))
		return false;
	return true;
}

static bool areValid(ArrayCompositions u, ArrayCompositions v) {
	return isValid(u) && isValid(v);
}

/* ************************************************************	*/
/*							incr								*/
/* ************************************************************	*/

ArrayCompositions& operator++(ArrayCompositions& composition) {

	if (!isValid(composition)) {
		std::cout << "ArrayCompositions& operator++(ArrayCompositions&) passed invalid parameter" << std::endl;
		composition = ArrayCompositions::ARRAY_COMPOSITIONS_COUNT;
	} else {
		int new_composition = static_cast<int>(composition);
		composition = static_cast<ArrayCompositions>(++new_composition);
	}
	return composition;
}

ArrayCompositions operator++(ArrayCompositions& composition, int unused) {

	// copy the existing state to pass back to the caller
	ArrayCompositions retval = composition;

	if (!isValid(composition)) {
		std::cout << "ArrayCompositions operator++(ArrayCompositions&, int) passed invalid parameter" << std::endl;
		composition = ArrayCompositions::ARRAY_COMPOSITIONS_COUNT;
	} else {
		++composition;
	}
	return retval;
}

int operator-(ArrayCompositions u, ArrayCompositions v) {

	if (!areValid(u,v))
		return 0;
	else
		return static_cast<int>(u) - static_cast<int>(v);
}

/* ************************************************************	*/
/*						relational operators					*/
/* ************************************************************	*/

//	required

bool operator==(ArrayCompositions u, ArrayCompositions v) {
	if (!areValid(u,v))
		return false;
	else
		return static_cast<int>(u) == static_cast<int>(v);
}

bool operator<(ArrayCompositions u, ArrayCompositions v) {
	if (!areValid(u,v))
		return false;
	else
		return static_cast<int>(u) - static_cast<int>(v) < 0;
}

//	derived

bool operator!=(ArrayCompositions u, ArrayCompositions v) {
	if (!areValid(u,v))
		return false;
	else
		return !(u == v);
}

bool operator<=(ArrayCompositions u, ArrayCompositions v) {
	if (!areValid(u,v))
		return false;
	else
		return (u < v) || (u == v);
}

bool operator>=(ArrayCompositions u, ArrayCompositions v) {
	if (!areValid(u,v))
		return false;
	else
		return  !(u < v);
}

bool operator>(ArrayCompositions u, ArrayCompositions v) {
	if (!areValid(u,v))
		return false;
	else
		return !(u < v) && !(u == v);
}

/* ************************************************************	*/
/*							formatted output					*/
/* ************************************************************	*/

std::string toInvalidString(ArrayCompositions composition) {

	std::stringstream ret;
	ret << "ERROR: static_cast<int>(ArrayCompositions composition) == " << static_cast<int>(composition);
	ret << " is not in range [" << static_cast<int>(MIN_ARRAY_COMPOSITION);
	ret << ":";
	ret << static_cast<int>(MAX_ARRAY_COMPOSITION);
	ret << "]";
	return ret.str();
}

std::ostream& operator<<(std::ostream &out, ArrayCompositions composition) {

	out << toString(composition);
	return out;
}

std::string toString(ArrayCompositions composition)
{
	if (!isValid(composition)) {
		return toInvalidString(composition);
	} else {
		return array_composition_strings[static_cast<int>(composition)];
	}
}

std::ostream& operator<<(std::ostream &out, const ArrayComposition &other) {
	out << other.str();
	return out;
}
