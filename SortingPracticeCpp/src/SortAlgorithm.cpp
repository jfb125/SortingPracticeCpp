/*
 * SortAlgorithm.cpp
 *
 *  Created on: Apr 16, 2025
 *      Author: joe
 */

#include "SortAlgorithm.h"

/* ************************************************************	*/
/*							safety								*/
/* ************************************************************	*/

bool isValid(SortAlgorithms algorithm) {

	switch(algorithm) {
	case SortAlgorithms::BUBBLE_SORT:
	case SortAlgorithms::SELECTION_SORT:
	case SortAlgorithms::INSERTION_SORT:
	case SortAlgorithms::MERGE_SORT:
	case SortAlgorithms::HEAP_SORT:
	case SortAlgorithms::QUICK_SORT:
	case SortAlgorithms::PROTECTED_QUICK_SORT:
	case SortAlgorithms::DUTCH_FLAG_SORT:
	case SortAlgorithms::RADIX_SORT:
	case SortAlgorithms::COUNTING_SORT:
	case SortAlgorithms::INPLACE_MERGE:
	case SortAlgorithms::BLOCK_SORT:
		return true;
		break;
	case SortAlgorithms::INVALID_SORT:
	default:
		return false;
	}
}

/*	***********************************************************	*/
/*						formatted output						*/
/*	***********************************************************	*/
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

std::string to_string(SortAlgorithms algorithm) {

	switch(algorithm) {
	case SortAlgorithms::BUBBLE_SORT:			return std::string("BUBBLE_SORT");
	case SortAlgorithms::SELECTION_SORT:		return std::string("SELECTION_SORT");
	case SortAlgorithms::INSERTION_SORT:		return std::string("INSERTION_SORT");
	case SortAlgorithms::MERGE_SORT:			return std::string("MERGE_SORT");
	case SortAlgorithms::HEAP_SORT:				return std::string("HEAP_SORT");
	case SortAlgorithms::QUICK_SORT:			return std::string("QUICK_SORT");
	case SortAlgorithms::PROTECTED_QUICK_SORT:	return std::string("PROTECTED_QUICK_SORT");
	case SortAlgorithms::DUTCH_FLAG_SORT:		return std::string("DUTCH_FLAG_SORT");
	case SortAlgorithms::RADIX_SORT:			return std::string("RADIX_SORT");
	case SortAlgorithms::COUNTING_SORT:			return std::string("COUNTING_SORT");
	case SortAlgorithms::INPLACE_MERGE:			return std::string("INPLACE_MERGE");
	case SortAlgorithms::BLOCK_SORT:			return std::string("BLOCK_SORT");
	case SortAlgorithms::INVALID_SORT:
	default: 									return std::string("UNKOWN_SORT");
	}
}

std::ostream& operator<<(std::ostream& out, SortAlgorithms algorithm) {

	out << to_string(algorithm);
	return out;
}
