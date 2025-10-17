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
#include <limits>

#include "SortingPracticeDataTypes.h"

#define ONE_SORT_REPETITIONS_WIDTH 0
#define ONE_SORT_COMPARES_WIDTH 0
#define ONE_SORT_SWAPS_WIDTH 0
#define ONE_SORT_AVERAGES_WIDTH 0
#define ONE_SORT_AVERAGES_HIGH_PRECISION 1
#define ONE_SORT_AVERAGES_MID_PRECISION 1
#define ONE_SORT_AVERAGES_LOW_PRECISION 1

class	SortTestMetrics {
public:
	total_compares_t compares;	// public so that ++ operator works
	total_moves_t assignments;		// public so that ++ operator works
	num_repetitions_t _num_repetitions;

	double averageCompares(void) const;
	double averageMoves(void) const;
	std::string averagesStr(void) const;	// verbose
	std::string totalCounts(void) const;
	std::string comparesStr(void) const;	// just the number
	std::string movesStr(void) const;		// just the number

	SortTestMetrics()	{ compares = 0; assignments = 0; _num_repetitions = 0; }
	~SortTestMetrics() {}
	SortTestMetrics(total_compares_t compares,
				  total_moves_t moves,
				  num_repetitions_t num_repetitions) :
					  compares(compares),
					  assignments(moves),
					  _num_repetitions(num_repetitions) {}

	SortTestMetrics(const SortTestMetrics &other);
	SortTestMetrics& operator=(const SortTestMetrics &other);
};

#endif /* SORTMETRICS_H_ */
