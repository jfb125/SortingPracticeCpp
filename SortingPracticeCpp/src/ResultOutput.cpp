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
