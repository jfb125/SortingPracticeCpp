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
#include "SortTestMetrics.h"
#include "OStreamState.h"

std::ostream& operator<<(std::ostream &out, const SortingDataType<int> &object) {
	out << object.value;
	return out;
}

std::ostream& operator<<(std::ostream &out, const SortingDataType<char> &object) {
	out << object.value;
	return out;
}

std::ostream& operator<<(std::ostream &out, const SortingDataType<std::string> &object) {
	out << object.value;
	return out;
}

std::ostream& operator<<(std::ostream &out, const SortMetrics& object) {
	out << std::setw(compares_width) 	<< object.compares
		<< " " << COMPARES_STRING << ", "
		<< std::setw(assignments_width) << object.assignments
		<< " " << ASSIGNMENTS_STRING;
	return out;
}

SortMetrics& operator+=(SortMetrics& lhs, const SortMetrics &rhs) {
	lhs.compares 	+= rhs.compares;
	lhs.assignments	+= rhs.assignments;
	return lhs;
}

std::string averageMetricsToString(	const SortMetrics &metrics,
									num_repetitions_t num_tests,
									int precision,
									int width) {
	OStreamState ostream_state;	// restores state on exit:
	std::stringstream result;
	result 	<< std::setprecision(precision) << std::setw(width)
			<< metrics.compares << " ave " << COMPARES_STRING << ", "
			<< std::setprecision(precision) << std::setw(width)
			<< metrics.assignments << " ave " << ASSIGNMENTS_STRING;
	return result.str();
}


std::string next_string(std::string &current, bool upper_then_lower){
	size_t str_size = current.size();
	std::string result = current;
	for (size_t i = str_size-1; i >= 0; i--) {
		if (upper_then_lower) {
			if (result[i] == 'z') {
				result[i] = 'A';
				continue;
			}
			if (result[i] == 'Z') {
				result[i] = 'a';
			}
			result[i] = result[i]+1;
			break;
		} else {
			if (result[i] == 'z') {
				result[i] = 'a';
				continue;
			}
			if (result[i] == 'Z') {
				result[i] = 'A';
				continue;
			}
			result[i] = result[i]+1;
			break;
		}
	}
	return result;
}

array_size_t generateStrings(std::string *dst,
							 array_size_t size,
							 std::string &first_string,
							 std::string (*next_string)(std::string&, bool)) {

	std::string value = first_string;
	for (array_size_t i = 0; i != size; i++) {
		dst[i] = value;
		value = next_string(value, false);
	}
	return size;
}





