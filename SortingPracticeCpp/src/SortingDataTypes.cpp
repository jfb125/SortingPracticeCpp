/*
 * SortingDataType.cpp
 *
 *  Created on: Oct 17, 2025
 *      Author: joe
 */

#include <iostream>
#include <iomanip>
#include <string>

#include "SortingDataTypes.h"
#include "OStreamState.h"

std::ostream& operator<<(std::ostream &out, const SortMetrics& object) {
	out << std::setw(compares_width) << object.compares << compares_str << ", "
		<< std::setw(assignments_width) << object.assignments << assignments_str;
	return out;
}

SortMetrics& operator+=(SortMetrics& lhs, const SortMetrics &rhs) {
	lhs.compares 	= rhs.compares;
	lhs.assignments	= rhs.assignments;
	return lhs;
}

std::string averageMetricsToString(	const SortMetrics &metrics,
									num_repetitions_t num_tests,
									int precision,
									int width) {
	OStreamState ostream_state;	// restores state on exit
	std::stringstream result;
	result 	<< std::setprecision(precision) << std::setw(width)
			<< metrics.compares << " ave " << compares_str << ", "
			<< std::setprecision(precision) << std::setw(width)
			<< metrics.assignments << " ave " << assignments_str;
	return result.str();
}





