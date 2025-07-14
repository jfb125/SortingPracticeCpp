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

class	SortMetrics {
public:
	total_compares_t _compares;	// public so that ++ operator works
	total_moves_t _moves;		// public so that ++ operator works
	num_repetitions_t _num_repetitions;

	double averageCompares(void) const;
	double averageMoves(void) const;
	std::string averagesStr(void) const;	// verbose
	std::string totalCounts(void) const;
	std::string comparesStr(void) const;	// just the number
	std::string movesStr(void) const;		// just the number

	SortMetrics()	{ _compares = 0; _moves = 0; _num_repetitions = 0; }
	~SortMetrics() {}
	SortMetrics(total_compares_t compares,
				  total_moves_t moves,
				  num_repetitions_t num_repetitions) :
					  _compares(compares),
					  _moves(moves),
					  _num_repetitions(num_repetitions) {}

	SortMetrics(const SortMetrics &other);
	SortMetrics& operator=(const SortMetrics &other);
};

#endif /* SORTMETRICS_H_ */
