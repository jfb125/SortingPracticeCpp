/*
 * OneSortResult.h
 *
 *  Created on: Apr 18, 2025
 *      Author: joe
 */

#ifndef SORTMETRICS_H_
#define SORTMETRICS_H_

#include <iostream>
#include <iomanip>
#include <ios>
#include <inttypes.h>
#include <climits>

#include "OStreamState.h"

using num_repetitions_t = long;
constexpr num_repetitions_t NUM_REPETITIONS_T_MIN = 0;
constexpr num_repetitions_t NUM_REPETITIONS_T_MAX = LONG_MAX;

using compares_t 	= long;
using assignments_t = long;

using total_compares_t = long long;
constexpr total_compares_t TOTAL_COMPARES_T_MIN = 0;
constexpr total_compares_t TOTAL_COMPARES_T_MAX = LONG_LONG_MAX;

using total_moves_t = long long;
constexpr total_moves_t	TOTAL_MOVES_T_MIN = 0;
constexpr total_moves_t TOTAL_MOVES_T_MAX = LONG_LONG_MAX;

constexpr total_moves_t num_assignments_per_swap = 3;

constexpr int compares_width 	= 8;
constexpr int assignments_width = 8;
#define	COMPARES_STRING 	"compares"
#define	ASSIGNMENTS_STRING	"assignments"

#include "SortingDataTypes.h"

#define ONE_SORT_REPETITIONS_WIDTH 0
#define ONE_SORT_COMPARES_WIDTH 0
#define ONE_SORT_SWAPS_WIDTH 0
#define ONE_SORT_AVERAGES_WIDTH 0
#define ONE_SORT_AVERAGES_HIGH_PRECISION 1
#define ONE_SORT_AVERAGES_MID_PRECISION 1
#define ONE_SORT_AVERAGES_LOW_PRECISION 1

//	This keeps track of the number of compares & assignments
//	which is a figure of merit for one sorting operation
class	SortMetrics {
public:
	compares_t		compares;
	assignments_t	assignments;

	SortMetrics() {	compares = 0; assignments = 0; }
	SortMetrics(compares_t cmp, assignments_t assgn) :
		compares(cmp),
		assignments(assgn) {}
	SortMetrics(const SortMetrics &other) {
		compares 	= other.compares;
		assignments	= other.assignments;
	}
	SortMetrics& operator=(const SortMetrics &other) {
		if (this != &other) {
			compares 	= other.compares;
			assignments = other.assignments;
		}
		return *this;
	}
	SortMetrics& operator+=(const SortMetrics &other) {
		if (this != &other) {
			compares	+= other.compares;
			assignments	+= other.assignments;
		}
		return *this;
	}
	friend std::ostream& operator<<(std::ostream& out, SortMetrics&object) {
		OStreamState ostream_state;
		out << std::setw(compares_width)
			<< std::right << object.compares
			<< " " << COMPARES_STRING << ", "
			<< std::setw(assignments_width)
			<< std::right << object.assignments
			<< " " << ASSIGNMENTS_STRING;
		return out;
	}
};

//	This keeps track of the cumulative performance of many runs of a sort
class	SortTestMetrics {
public:
	total_compares_t 	compares;			// public so that ++ operator works
	total_moves_t 		assignments;		// public so that ++ operator works
	num_repetitions_t 	num_repetitions;
	bool				is_stable;			// defaults to true

	double averageCompares(void) const;
	double averageAssignments(void) const;
	std::string averages_str(void) const;		// verbose
	std::string totalCounts(void) const;
	std::string foobar(void) const;
	std::string compares_str(void) const;
	std::string assignments_str(void) const;	// just the number

	SortTestMetrics() { compares 		= 0;
					  	assignments 	= 0;
					  	num_repetitions = 0;
					  	is_stable 		= true;}

	~SortTestMetrics() {}
	SortTestMetrics(total_compares_t 	_compares,
				  	total_moves_t 		_moves,
				  	num_repetitions_t 	_num_repetitions,
		 			bool				_is_stable =  true) :
					  compares(_compares),
					  assignments(_moves),
					  num_repetitions(_num_repetitions),
					  is_stable(_is_stable) {}

	SortTestMetrics(const SortTestMetrics &other);
	SortTestMetrics& operator=(const SortTestMetrics &other);

	SortTestMetrics& operator+=(const SortMetrics &object);
};

#endif /* SORTMETRICS_H_ */
