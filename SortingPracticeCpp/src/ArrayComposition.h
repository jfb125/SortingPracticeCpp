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
	ArrayCompositions _composition;
	// number of discrete values in an array of FEW_DISCRETE
	//	i.e. in an array of size 12 with num_discrete = 4
	//	[] = { A, A, A, A, B, B, B, B, C, C, C, C, D, D, D, D }
	int _num_discrete_values;
	// number of values that differ from common value with FEW_DIFFERENT
	//	i.e. in an array of size 12 with num_different = 4
	//	[] = { A, A, A, A, A, A, A, A, A, A, A, A, B, C, D, E }
	int _num_different;

	// ctors, dtor & assignment operators, std::move() not implemented intentionally

	ArrayComposition() {
		_composition = DEFAULT_ARRAY_COMPOSITION;
		_num_discrete_values = DEFAULT_NUM_DISCRETE_VALUES;
		_num_different = DEFAULT_NUM_DIFFERENT_VALUES;
	}

	ArrayComposition(ArrayCompositions comp,
					 int _discrete = DEFAULT_NUM_DISCRETE_VALUES,
					 int _diff = DEFAULT_NUM_DIFFERENT_VALUES) :
		_composition(comp),
		_num_discrete_values(_discrete),
		_num_different(_diff) {}

	ArrayComposition& operator=(const ArrayComposition& other) {
		if (this != &other) {
			_composition = other._composition;
			_num_discrete_values = other._num_discrete_values;
			_num_different = other._num_different;
		}
		return *this;
	}

	// this does not change the values of _num_discrete & _num_different
	ArrayComposition& operator=(ArrayCompositions composition) {
		_composition = composition;
		return *this;
	}

	//	increment operators

	ArrayComposition& operator++() {
		++_composition;
		return *this;
	}

	ArrayComposition operator++(int unused) {
		ArrayComposition retval(*this);
		++_composition;
		return retval;
	}

	//	arithmetic operators

	int operator-(const ArrayComposition &other) const {
		return _composition - other._composition;
	}

	// relational operators

	bool operator==(const ArrayComposition &other) const {
		return _composition == other._composition;
	}
	bool operator<(const ArrayComposition &other) const {
		return _composition < other._composition;
	}
	bool operator!=(const ArrayComposition &other) const {
		return !(_composition == other._composition);
	}
	bool operator<=(const ArrayComposition &other) const {
		return _composition == other._composition ||
				_composition < other._composition;
	}
	bool operator>=(const ArrayComposition & other) const {
		return !(_composition < other._composition);
	}
	bool operator>(const ArrayComposition & other) const {
		return !(_composition == other._composition ||
				 _composition < other._composition);
	}

	//	formatted output

	std::string str(void) const {

		std::stringstream retval;
		retval << _composition;
		switch(_composition) {
		case ArrayCompositions::FEW_DIFFERENT:
			retval << " with " << _num_different << " changed elements";
			break;
		case ArrayCompositions::FEW_DISCRETE:
			retval << " with " << this->_num_discrete_values << " possible values";
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
