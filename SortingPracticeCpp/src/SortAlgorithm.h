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

#include "SortingDataTypes.h"
#include "SortTestMetrics.h"

/*	**************************************************************************	*/
/*	**************************************************************************	*/
/*	 						supported sort algorithms							*/
/*	**************************************************************************	*/
/*	**************************************************************************	*/

template <typename T>
using SortFunction = void (*)(T* array_size_t, SortMetrics*);

enum class SortAlgorithms {
	BUBBLE_SORT,
	SELECTION_SORT,
	INSERTION_SORT,
	MERGE_SORT,
	HEAP_SORT,
	QUICK_SORT,
	OPTIMIZED_QUICK_SORT,
	DUTCH_FLAG_SORT,
	RADIX_SORT,
	COUNTING_SORT,
	INPLACE_MERGE,
	BLOCK_SORT,
	INVALID_SORT,
};

bool isValid(SortAlgorithms);

std::ostream& operator<<(std::ostream&, SortAlgorithms);
std::string to_string(SortAlgorithms);

#endif /* SORTALGORITHM_H_ */
