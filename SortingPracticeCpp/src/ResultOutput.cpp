/*
 * ResultOutputParameters.cpp
 *
 *  Created on: Jun 21, 2025
 *      Author: joe
 */

#include "ResultOutput.h"

void printRowStart(SortAlgorithms &algorithm, ArrayComposition &composition, InitialOrdering &ordering,
					int algorithm_strlen, int composition_strlen, int ordering_strlen) {

	std::cout << std::setw(algorithm_strlen)
			  << std::left << algorithm << space_separator
			  << std::setw(composition_strlen)
			  << std::left << composition << space_separator
			  << std::setw(ordering_strlen)
			  << std::left << ordering << colon_separator;
}
