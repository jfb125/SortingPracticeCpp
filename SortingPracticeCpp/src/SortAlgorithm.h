/*
 * SortAlgorithm.h
 *
 *  Created on: Apr 16, 2025
 *      Author: joe
 */

#ifndef SORTALGORITHM_H_
#define SORTALGORITHM_H_

#include <iostream>
#include <sstream>
#include <string>

/*	**************************************************************************	*/
/*	**************************************************************************	*/
/*	 						supported sort algorithms							*/
/*	**************************************************************************	*/
/*	**************************************************************************	*/

#undef _SORT_ALGORITHM
#define _SORT_ALGORITHMS\
	_SORT_ALGORITHM(BUBBLE_SORT)\
	_SORT_ALGORITHM(SELECTION_SORT)\
	_SORT_ALGORITHM(INSERTION_SORT)\
	_SORT_ALGORITHM(MERGE_SORT)\
	_SORT_ALGORITHM(HEAP_SORT)\
	_SORT_ALGORITHM(QUICK_SORT)\
	_SORT_ALGORITHM(OPTIMIZED_QUICK_SORT)\
	_SORT_ALGORITHM(DUTCH_FLAG_SORT)\
	_SORT_ALGORITHM(RADIX_SORT)\
	_SORT_ALGORITHM(COUNTING_SORT)\
	_SORT_ALGORITHM(BLOCK_SORT)\
	_SORT_ALGORITHM(SORT_ALGORITHMS_COUNT)	// place holder

#define _SORT_ALGORITHM(alg)	alg,
enum SortAlgorithms {
	_SORT_ALGORITHMS
};

#define MIN_SORT_ALGORITHM 		SortAlgorithms::BUBBLE_SORT
#define MAX_SORT_ALGORITHM 		SortAlgorithms::BLOCK_SORT
#define INVALID_SORT_ALGORITHM	SortAlgorithms::SORT_ALGORITHMS_COUNT

bool isValid(SortAlgorithms);

SortAlgorithms& operator++(SortAlgorithms& algorithm);
SortAlgorithms operator++(SortAlgorithms& algorithm, int unused);

int operator-(SortAlgorithms u, SortAlgorithms v);

bool operator==(SortAlgorithms u, SortAlgorithms v);
bool operator<(SortAlgorithms u, SortAlgorithms v);

bool operator!=(SortAlgorithms u, SortAlgorithms v);
bool operator<=(SortAlgorithms u, SortAlgorithms v);
bool operator>=(SortAlgorithms u, SortAlgorithms v);
bool operator>(SortAlgorithms u, SortAlgorithms v);

std::ostream& operator<<(std::ostream&, SortAlgorithms);
std::string toString(SortAlgorithms);

#endif /* SORTALGORITHM_H_ */
