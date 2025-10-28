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

//	ALL_PERMUTATIONS being lowest improves look of ResultOutput table
enum class ArrayCompositions {
		ALL_DISCRETE,
		ALL_SAME,
		FEW_DIFFERENT,
		FEW_DISTINCT,
		ALL_PERMUTATIONS,
	};

//	fundamental
bool operator==(const ArrayCompositions u, const ArrayCompositions v);
bool operator< (const ArrayCompositions u, const ArrayCompositions v);
//	derived
bool operator<=(const ArrayCompositions u, const ArrayCompositions v);
bool operator> (const ArrayCompositions u, const ArrayCompositions v);
bool operator>=(const ArrayCompositions u, const ArrayCompositions v);
bool operator!=(const ArrayCompositions u, const ArrayCompositions v);

constexpr int array_composition_string_length = 16;

namespace std {
	std::string to_string(ArrayCompositions composition);
}

std::ostream& operator<<(std::ostream& out, ArrayCompositions composition);

#define DEFAULT_ARRAY_COMPOSITION		ArrayCompositions::ALL_DISCRETE
#define DEFAULT_NUM_DIFFERENT_VALUES	1
#define DEFAULT_NUM_DISTINCT_VALUES 	1

class ArrayComposition {
public:
	ArrayCompositions composition;
	// number of discrete values in an array of FEW_DISCRETE
	//	i.e. in an array of size 12 with num_discrete = 4
	//	[] = { A, A, A, A, B, B, B, B, C, C, C, C, D, D, D, D }
	int num_distinct_values;
	// number of values that differ from common value with FEW_DIFFERENT
	//	i.e. in an array of size 12 with num_different = 4
	//	[] = { A, A, A, A, A, A, A, A, A, A, A, A, B, C, D, E }
	int num_different;

	// ctors, dtor & assignment operators, std::move() not implemented intentionally

	ArrayComposition() {
		composition 		= DEFAULT_ARRAY_COMPOSITION;
		num_distinct_values = DEFAULT_NUM_DISTINCT_VALUES;
		num_different 		= DEFAULT_NUM_DIFFERENT_VALUES;
	}

	ArrayComposition(ArrayCompositions comp,
					 int x_num_distinct = DEFAULT_NUM_DISTINCT_VALUES,
					 int x_diff 		= DEFAULT_NUM_DIFFERENT_VALUES) :
		composition(comp),
		num_distinct_values(x_num_distinct),
		num_different(x_diff) {}

	ArrayComposition& operator=(const ArrayComposition& other) {
		if (this != &other) {
			composition 		= other.composition;
			num_distinct_values = other.num_distinct_values;
			num_different 		= other.num_different;
		}
		return *this;
	}

	// this does not change the values of _num_discrete & _num_different
	ArrayComposition& operator=(ArrayCompositions x_composition) {
		composition = x_composition;
		return *this;
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

	std::string to_string(void) const {

		std::stringstream retval;
		retval << composition;
		switch(composition) {
		case ArrayCompositions::FEW_DIFFERENT:
			retval << " with " << num_different << " changed elements";
			break;
		case ArrayCompositions::FEW_DISTINCT:
			retval << " with " << num_distinct_values << " possible values";
			break;
		case ArrayCompositions::ALL_DISCRETE:
		case ArrayCompositions::ALL_SAME:
		case ArrayCompositions::ALL_PERMUTATIONS:
		default:
			break;
		}
		return retval.str();
	}
};

std::ostream& operator<<(std::ostream &out, const ArrayComposition &object);

#endif /* ARRAYCOMPOSITION_H_ */
