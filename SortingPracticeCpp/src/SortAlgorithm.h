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
	PROTECTED_QUICK_SORT,
	DUTCH_FLAG_SORT,
	RADIX_SORT,
	COUNTING_SORT,
	INPLACE_MERGE,
	BLOCK_SORT,
	INVALID_SORT,
};

#define SORT_ALGORITHM_BUBBLE_SORT_STRING			"BUBBLE SORT"
#define SORT_ALGORITHM_SELECTION_SORT_STRING		"SELECTION SORT"
#define SORT_ALGORITHM_INSERTION_SORT_STRING		"INSERTION SORT"
#define SORT_ALGORITHM_MERGE_SORT_STRING			"MERGE SORT"
#define SORT_ALGORITHM_HEAP_SORT_STRING				"HEAP SORT"
#define SORT_ALGORITHM_QUICK_SORT_STRING			"QUICK SORT"
#define SORT_ALGORITHM_PROTECTED_QUICK_SORT_STRING	"PROTECTED QUCK SORT"
#define SORT_ALGORITHM_DUTCH_FLAG_SORT_STRING		"DUTCH SORT"
#define SORT_ALGORITHM_RADIX_SORT_STRING			"RADIX SORT"
#define SORT_ALGORITHM_COUNTING_SORT_STRING			"COUNTING SORT"
#define SORT_ALGORITHM_INPLACE_MERGE_SORT_STRING	"INPLACE MERGE SORT"
#define SORT_ALGORITHM_BLOCK_SORT_STRING			"BLOCK SORT"
#define SORT_ALGORITHM_INVALID_SORT_STRING			"INVALID SORT"

bool isValid(SortAlgorithms);

std::ostream& operator<<(std::ostream&, SortAlgorithms);
std::string to_string(SortAlgorithms);

#endif /* SORTALGORITHM_H_ */
