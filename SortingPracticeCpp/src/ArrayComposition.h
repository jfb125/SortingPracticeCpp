/*
 * Array_Composition.h
 *
 *  Created on: Apr 16, 2025
 *      Author: joe
 */

#ifndef ARRAYCOMPOSITION_H_
#define ARRAYCOMPOSITION_H_

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#undef _ARRAY_COMPOSITION
#define _ARRAY_COMPOSITIONS\
	_ARRAY_COMPOSITION(ALL_DISCRETE)\
	_ARRAY_COMPOSITION(ALL_SAME)\
	_ARRAY_COMPOSITION(FEW_DISCRETE)\
	_ARRAY_COMPOSITION(FEW_DIFFERENT)\
	_ARRAY_COMPOSITION(ARRAY_COMPOSITIONS_COUNT)

#define MIN_ARRAY_COMPOSITION 		ArrayCompositions::ALL_DISCRETE
#define MAX_ARRAY_COMPOSITION 		ArrayCompositions::FEW_DIFFERENT
#define INVALID_ARRAY_COMPOSITION 	ArrayCompositions::ARRAY_COMPOSITIONS_COUNT

#define _ARRAY_COMPOSITION(composition)	composition,
enum ArrayCompositions {
	_ARRAY_COMPOSITIONS
};

bool isValid(ArrayCompositions);

ArrayCompositions& operator++(ArrayCompositions& current_composition);
ArrayCompositions operator++(ArrayCompositions& current_composition, int);

int operator-(ArrayCompositions u, ArrayCompositions v);

bool operator==(ArrayCompositions u, ArrayCompositions v);
bool operator<(ArrayCompositions u, ArrayCompositions v);

bool operator!=(ArrayCompositions u, ArrayCompositions v);
bool operator<=(ArrayCompositions u, ArrayCompositions v);
bool operator>(ArrayCompositions u, ArrayCompositions v);
bool operator>=(ArrayCompositions u, ArrayCompositions v);

std::string toString(ArrayCompositions);
std::ostream& operator<<(std::ostream&, ArrayCompositions);

#define DEFAULT_ARRAY_COMPOSITION ArrayCompositions::ALL_SAME
#define DEFAULT_NUM_DISCRETE_VALUES	1	/* creates all same values	*/
#define DEFAULT_NUM_DIFFERENT_VALUES 0	/* creates all same values	*/

class ArrayComposition {
public:
	ArrayCompositions composition;
	// number of discrete values in an array of FEW_DISCRETE
	//	i.e. in an array of size 12 with num_discrete = 4
	//	[] = { A, A, A, A, B, B, B, B, C, C, C, C, D, D, D, D }
	int num_discrete_values;
	// number of values that differ from common value with FEW_DIFFERENT
	//	i.e. in an array of size 12 with num_different = 4
	//	[] = { A, A, A, A, A, A, A, A, A, A, A, A, B, C, D, E }
	int num_different;

	// ctors, dtor & assignment operators, std::move() not implemented intentionally

	ArrayComposition() {
		composition 		= DEFAULT_ARRAY_COMPOSITION;
		num_discrete_values = DEFAULT_NUM_DISCRETE_VALUES;
		num_different 		= DEFAULT_NUM_DIFFERENT_VALUES;
	}

	ArrayComposition(ArrayCompositions comp,
					 int x_discrete = DEFAULT_NUM_DISCRETE_VALUES,
					 int x_diff 	= DEFAULT_NUM_DIFFERENT_VALUES) :
		composition(comp),
		num_discrete_values(x_discrete),
		num_different(x_diff) {}

	ArrayComposition& operator=(const ArrayComposition& other) {
		if (this != &other) {
			composition 		= other.composition;
			num_discrete_values = other.num_discrete_values;
			num_different 		= other.num_different;
		}
		return *this;
	}

	// this does not change the values of _num_discrete & _num_different
	ArrayComposition& operator=(ArrayCompositions x_composition) {
		composition = x_composition;
		return *this;
	}

	//	increment operators

	ArrayComposition& operator++() {
		++composition;
		return *this;
	}

	ArrayComposition operator++(int unused) {
		ArrayComposition retval(*this);
		++composition;
		return retval;
	}

	//	arithmetic operators

	int operator-(const ArrayComposition &other) const {
		return composition - other.composition;
	}

	// relational operators

	bool operator==(const ArrayComposition &other) const {
		return composition == other.composition;
	}
	bool operator<(const ArrayComposition &other) const {
		return composition < other.composition;
	}
	bool operator!=(const ArrayComposition &other) const {
		return !(composition == other.composition);
	}
	bool operator<=(const ArrayComposition &other) const {
		return composition == other.composition ||
				composition < other.composition;
	}
	bool operator>=(const ArrayComposition & other) const {
		return !(composition < other.composition);
	}
	bool operator>(const ArrayComposition & other) const {
		return !(composition == other.composition ||
				 composition < other.composition);
	}

	//	formatted output

	std::string str(void) const {

		std::stringstream retval;
		retval << composition;
		switch(composition) {
		case ArrayCompositions::FEW_DIFFERENT:
			retval << " with " << num_different << " changed elements";
			break;
		case ArrayCompositions::FEW_DISCRETE:
			retval << " with " << this->num_discrete_values << " possible values";
			break;
		case ArrayCompositions::ALL_DISCRETE:
		case ArrayCompositions::ALL_SAME:
		default:
			break;
		}
		return retval.str();
	}
};

std::ostream& operator<<(std::ostream &out, const ArrayComposition &other);

#endif /* ARRAYCOMPOSITION_H_ */
