/*
 * ResultOutputParameters.cpp
 *
 *  Created on: Jun 21, 2025
 *      Author: joe
 */

#include "ResultOutput.h"

std::string rowPreambleToString(SortAlgorithms &algorithm, ArrayComposition &composition, InitialOrdering &ordering,
								int algorithm_strlen, int composition_strlen, int ordering_strlen,
								ResultTableOrdering &structure) {

	std::stringstream result;
	switch(structure.first()) {
	default:
	case ResultTableElements::ALGORITHM:
		result 	<< std::setw(algorithm_strlen) 	<< std::left << algorithm 	<< space_separator;
		break;
	case ResultTableElements::COMPOSITION:
		result	<< std::setw(composition_strlen)<< std::left << composition << space_separator;
		break;
	case ResultTableElements::ORDERING:
		result	<< std::setw(ordering_strlen) 	<< std::left << ordering 	<< space_separator;
		break;
	}

	switch(structure.second()) {
	case ResultTableElements::ALGORITHM:
		result 	<< std::setw(algorithm_strlen) 	<< std::left << algorithm 	<< space_separator;
		break;
	default:
	case ResultTableElements::COMPOSITION:
		result	<< std::setw(composition_strlen)<< std::left << composition << space_separator;
		break;
	case ResultTableElements::ORDERING:
		result	<< std::setw(ordering_strlen) 	<< std::left << ordering 	<< space_separator;
		break;
	}

	switch(structure.third()) {
	case ResultTableElements::ALGORITHM:
		result 	<< std::setw(algorithm_strlen) 	<< std::left << algorithm 	<< space_separator;
		break;
	case ResultTableElements::COMPOSITION:
		result	<< std::setw(composition_strlen)<< std::left << composition << space_separator;
		break;
	default:
	case ResultTableElements::ORDERING:
		result	<< std::setw(ordering_strlen) 	<< std::left << ordering 	<< space_separator;
		break;
	}

	return result.str();
}


std::string stabilityToString(ArrayCompositions composition, bool stable)
{
	std::stringstream retval;

	switch(composition) {
	//	if there are no identical elements, there is not question
	//	of whether or not the common elements preserved their order
	case ArrayCompositions::ALL_DISCRETE:
	case ArrayCompositions::ALL_PERMUTATIONS:
		retval << is_not_applicable_stability_string;
		break;
	case ArrayCompositions::FEW_DIFFERENT:
	case ArrayCompositions::ALL_SAME:
	case ArrayCompositions::FEW_DISTINCT:
		if (stable) {
			retval << is_stable_string;
		} else {
			retval << is_not_stable_string;
		}
	}
	return retval.str();
}

