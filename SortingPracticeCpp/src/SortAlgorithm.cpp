/*
 * SortAlgorithm.cpp
 *
 *  Created on: Apr 16, 2025
 *      Author: Joe Baker
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

std::string to_string(SortAlgorithms algorithm) {

	switch(algorithm) {
	case SortAlgorithms::BUBBLE_SORT:			return std::string(SORT_ALGORITHM_BUBBLE_SORT_STRING);
	case SortAlgorithms::SELECTION_SORT:		return std::string(SORT_ALGORITHM_SELECTION_SORT_STRING);
	case SortAlgorithms::INSERTION_SORT:		return std::string(SORT_ALGORITHM_INSERTION_SORT_STRING);
	case SortAlgorithms::MERGE_SORT:			return std::string(SORT_ALGORITHM_MERGE_SORT_STRING);
	case SortAlgorithms::HEAP_SORT:				return std::string(SORT_ALGORITHM_HEAP_SORT_STRING);
	case SortAlgorithms::QUICK_SORT:			return std::string(SORT_ALGORITHM_QUICK_SORT_STRING);
	case SortAlgorithms::PROTECTED_QUICK_SORT:	return std::string(SORT_ALGORITHM_PROTECTED_QUICK_SORT_STRING);
	case SortAlgorithms::DUTCH_FLAG_SORT:		return std::string(SORT_ALGORITHM_DUTCH_FLAG_SORT_STRING);
	case SortAlgorithms::RADIX_SORT:			return std::string(SORT_ALGORITHM_RADIX_SORT_STRING);
	case SortAlgorithms::COUNTING_SORT:			return std::string(SORT_ALGORITHM_COUNTING_SORT_STRING);
	case SortAlgorithms::INPLACE_MERGE:			return std::string(SORT_ALGORITHM_INPLACE_MERGE_SORT_STRING);
	case SortAlgorithms::BLOCK_SORT:			return std::string(SORT_ALGORITHM_BLOCK_SORT_STRING);
	case SortAlgorithms::INVALID_SORT:
	default: 									return std::string(SORT_ALGORITHM_INVALID_SORT_STRING);
	}
}

std::ostream& operator<<(std::ostream& out, SortAlgorithms algorithm) {

	out << to_string(algorithm);
	return out;
}
