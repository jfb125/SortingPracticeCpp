/*
 * OneSortResult.cpp
 *
 *  Created on: Apr 18, 2025
 *      Author: joe
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <climits>
#include <limits>
#include <inttypes.h>

#include "OStreamState.h"
#include "SortTestMetrics.h"

double	SortTestMetrics::averageCompares(void) const {

	if (num_repetitions) {
		return static_cast<double>(compares)/num_repetitions;
	} else {
		return std::numeric_limits<double>::max();
	}
}

double	SortTestMetrics::averageAssignments(void) const {

	if (num_repetitions) {
		return static_cast<double>(assignments)/num_repetitions;
	} else {
		return std::numeric_limits<double>::max();
	}
}

std::string SortTestMetrics::averages_str(void) const {

	OStreamState ostream_state;	// restores stat in destructor

	std::stringstream retval;

	double average_compares = averageCompares();
	double average_swaps	= averageAssignments();

	int precision = 0;
	if (average_compares < 100.0)		precision = ONE_SORT_AVERAGES_HIGH_PRECISION;
	else if(average_compares < 1000.0)	precision = ONE_SORT_AVERAGES_MID_PRECISION;
	else								precision = ONE_SORT_AVERAGES_LOW_PRECISION;

	retval << std::fixed << std::setw(ONE_SORT_AVERAGES_WIDTH)
			<< std::setprecision(precision)
			<< "average compares: "	<< average_compares
			<< ", average swaps: " 	<< average_swaps;

	return retval.str();
}


std::string SortTestMetrics::compares_str(void) const {

	OStreamState ostream_state;	// restores ostream flags in destructor

	std::stringstream retval;

	double average_compares = averageCompares();

	int precision = 0;
	if (average_compares < 100.0)		precision = ONE_SORT_AVERAGES_HIGH_PRECISION;
	else if(average_compares < 1000.0)	precision = ONE_SORT_AVERAGES_MID_PRECISION;
	else								precision = ONE_SORT_AVERAGES_LOW_PRECISION;

	retval << std::fixed << std::setw(ONE_SORT_AVERAGES_WIDTH)
			<< std::setprecision(precision)
			<< average_compares;

	return retval.str();
}

std::string SortTestMetrics::assignments_str(void) const {

	OStreamState ostream_state;		// restores flags in its destructor

	std::stringstream retval;

	double average_assignments = averageAssignments();

	int precision = 0;
	if (average_assignments < 100.0)		precision = ONE_SORT_AVERAGES_HIGH_PRECISION;
	else if(average_assignments < 1000.0)	precision = ONE_SORT_AVERAGES_MID_PRECISION;
	else									precision = ONE_SORT_AVERAGES_LOW_PRECISION;

	retval << std::fixed << std::setw(ONE_SORT_AVERAGES_WIDTH)
			<< std::setprecision(precision)
			<< average_assignments;

	return retval.str();
}

std::string SortTestMetrics::totalCounts(void) const {

	OStreamState ostream_state;	//	restores ostream flags in its destructor
	std::stringstream retval;

	retval 	<< "repeats: " 		<< std::setw(ONE_SORT_REPETITIONS_WIDTH) << std::right
			<< num_repetitions;
	retval	<< ", compares: " 	<< std::setw(ONE_SORT_COMPARES_WIDTH) << std::right
			<< compares;
	retval	<< ", swaps: " 		<< std::setw(ONE_SORT_SWAPS_WIDTH) << std::right
			<< assignments;

	return retval.str();
}

SortTestMetrics::SortTestMetrics(const SortTestMetrics &other) {

	compares 		= other.compares;
	assignments 	= other.assignments;
	num_repetitions = other.num_repetitions;
	is_stable		= other.is_stable;
}

SortTestMetrics& SortTestMetrics::operator=(const SortTestMetrics& other) {

	if (this != &other) {
		compares 		= other.compares;
		assignments 	= other.assignments;
		num_repetitions = other.num_repetitions;
		//	do not overwrite 'is_stable' if a previous operation has
		//	determined that the algorithm was not stable
		//	- i.e, 'is_stable' is sticky once false
		if (is_stable)
			is_stable		= other.is_stable;
	}
	return *this;
}

SortTestMetrics& SortTestMetrics::operator+=(const SortMetrics &object) {
	compares 	+= object.compares;
	assignments	+= object.assignments;
	return *this;
}
