/*
 * Array_Composition.cpp
 *
 *  Created on: Apr 16, 2025
 *      Author: joe
 */

#include "ArrayComposition.h"

bool isValid(ArrayCompositions composition) {
	switch (composition) {
	case ArrayCompositions::ALL_DISCRETE:
	case ArrayCompositions::ALL_PERMUTATIONS:
	case ArrayCompositions::ALL_SAME:
	case ArrayCompositions::FEW_DIFFERENT:
	case ArrayCompositions::FEW_DISTINCT:
		return true;
	default:
		return false;
	}
}

//	fundamental
bool operator==(const ArrayCompositions u, const ArrayCompositions v) {
	return static_cast<int>(u) == static_cast<int>(v);
}
bool operator< (const ArrayCompositions u, const ArrayCompositions v) {
	return static_cast<int>(u) < static_cast<int>(v);
}
//	derived
bool operator<=(const ArrayCompositions u, const ArrayCompositions v) {
	return (u == v) || (u < v);
}
bool operator> (const ArrayCompositions u, const ArrayCompositions v) {
	return !(u == v) && !(u < v);
}
bool operator>=(const ArrayCompositions u, const ArrayCompositions v) {
	return !(u < v);
}
bool operator!=(const ArrayCompositions u, const ArrayCompositions v) {
	return !(u == v);
}

std::string std::to_string(ArrayCompositions composition) {
	std::stringstream result;
	switch(composition) {
	case ArrayCompositions::ALL_DISCRETE:
		result << "ALL_DISCRETE";
		break;
	case ArrayCompositions::ALL_PERMUTATIONS:
		result << "ALL_PERMUTATIONS";
		break;
	case ArrayCompositions::ALL_SAME:
		result << "ALL_SAME";
		break;
	case ArrayCompositions::FEW_DIFFERENT:
		result << "FEW_DIFFERENT";
		break;
	case ArrayCompositions::FEW_DISTINCT:
		result << "FEW_DISTINCT";
		break;
	default:
		result << "INVALID_ARRAY_COMPOSITION";
		break;
	}
	return result.str();
}

std::ostream& operator<<(std::ostream& out, ArrayCompositions composition) {
	out << std::to_string(composition);
	return out;
}

std::ostream& operator<<(std::ostream &out, const ArrayComposition &object) {
	out << object.to_string();
	return out;
}
