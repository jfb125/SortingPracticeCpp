/*
 * OneSortResult.cpp
 *
 *  Created on: Apr 18, 2025
 *      Author: joe
 */

#include "SortMetrics.h"

double	SortMetrics::averageCompares(void) const {

	if (_num_repetitions) {
		return static_cast<double>(_compares)/_num_repetitions;
	} else {
		return std::numeric_limits<double>::max();
	}
}

double	SortMetrics::averageMoves(void) const {

	if (_num_repetitions) {
		return static_cast<double>(_moves)/_num_repetitions;
	} else {
		return std::numeric_limits<double>::max();
	}
}

std::string SortMetrics::averagesStr(void) const {

	// ensure that the previously existing value of left vs right is maintained
	std::ios_base::fmtflags _flags = std::cout.flags();

	std::stringstream retval;
//	retval 	<< "repeats: " 	<< std::setw(ONE_SORT_REPETITIONS_WIDTH) << std::right
//			<< _num_repetitions << " ";

	double average_compares = averageCompares();
	double average_swaps	= averageMoves();

	int precision = 0;
	if (average_compares < 100.0)		precision = ONE_SORT_AVERAGES_HIGH_PRECISION;
	else if(average_compares < 1000.0)	precision = ONE_SORT_AVERAGES_MID_PRECISION;
	else								precision = ONE_SORT_AVERAGES_LOW_PRECISION;

	retval << std::fixed << std::setw(ONE_SORT_AVERAGES_WIDTH)
			<< std::setprecision(precision)
			<< "average compares: "	<< average_compares
			<< ", average swaps: " 	<< average_swaps;

	// restore flags
	std::cout.flags(_flags);
	return retval.str();
}
std::string SortMetrics::comparesStr(void) const {

	// ensure that the previously existing value of left vs right is maintained
	std::ios_base::fmtflags _flags = std::cout.flags();

	std::stringstream retval;

	double average_compares = averageCompares();

	int precision = 0;
	if (average_compares < 100.0)		precision = ONE_SORT_AVERAGES_HIGH_PRECISION;
	else if(average_compares < 1000.0)	precision = ONE_SORT_AVERAGES_MID_PRECISION;
	else								precision = ONE_SORT_AVERAGES_LOW_PRECISION;

	retval << std::fixed << std::setw(ONE_SORT_AVERAGES_WIDTH)
			<< std::setprecision(precision)
			<< average_compares;

	// restore flags
	std::cout.flags(_flags);
	return retval.str();
}

std::string SortMetrics::movesStr(void) const {

	// ensure that the previously existing value of left vs right is maintained
	std::ios_base::fmtflags _flags = std::cout.flags();

	std::stringstream retval;

	double average_swaps = averageMoves();

	int precision = 0;
	if (average_swaps < 100.0)		precision = ONE_SORT_AVERAGES_HIGH_PRECISION;
	else if(average_swaps < 1000.0)	precision = ONE_SORT_AVERAGES_MID_PRECISION;
	else							precision = ONE_SORT_AVERAGES_LOW_PRECISION;

	retval << std::fixed << std::setw(ONE_SORT_AVERAGES_WIDTH)
			<< std::setprecision(precision)
			<< average_swaps;

	// restore flags
	std::cout.flags(_flags);
	return retval.str();
}

std::string SortMetrics::totalCounts(void) const {

	std::stringstream retval;

	// ensure that the previously existing value of left vs right is maintained
	std::ios_base::fmtflags _flags = std::cout.flags();

	retval 	<< "repeats: " 	<< std::setw(ONE_SORT_REPETITIONS_WIDTH) << std::right
			<< _num_repetitions;
	retval	<< ", compares: " << std::setw(ONE_SORT_COMPARES_WIDTH) << std::right
			<< _compares;
	retval	<< ", swaps: " 		<< std::setw(ONE_SORT_SWAPS_WIDTH) << std::right
			<< _moves;

	// restore flags
	std::cout.flags(_flags);
	return retval.str();
}

SortMetrics::SortMetrics(const SortMetrics &other) {

	if (this != &other) {
		_compares = other._compares;
		_moves = other._moves;
		_num_repetitions = other._num_repetitions;
	}
}

SortMetrics& SortMetrics::operator=(const SortMetrics& other) {

	if (this != &other) {
		_compares = other._compares;
		_moves = other._moves;
		_num_repetitions = other._num_repetitions;
	}
	return *this;
}
