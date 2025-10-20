/*
 * Sort_Test.h
 *
 *  Created on: Apr 15, 2025
 *      Author: joe
 */

#ifndef SORTTEST_H_
#define SORTTEST_H_

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstddef>
#include <cstdlib>
#include <ctime>

#include "ArrayComposition.h"
#include "Disorganizer.h"
#include "SortFailureLog.h"
#include "InitialOrdering.h"
#include "MessageList.h"
#include "OneTestResult.h"		// results of running n tests of a given array length
#include "ResultOutput.h"
#include "SimpleRandomizer.h"
#include "SortableObject.h"
#include "SortAlgorithm.h"
#include "SortingDataTypes.h"

#include "SortingUtilities.h"

#include "BlockSort.h"
#include "BubbleSort.h"
#include "DutchFlagSort.h"
#include "InPlaceMerge.h"
#include "InsertionSort.h"
#include "SelectionSort.h"
#include "MergeSort.h"
#include "HeapSort.h"
#include "OptimizedQuickSort.h"
#include "QuickSort.h"


template <typename T>
void	printSideBySide(SortingDataType<T> &a, array_size_t a_size,
						SortingDataType<T> &b, array_size_t b_size) {

	array_size_t a_i = 0;
	array_size_t b_i = 0;
	for (; a_i < a_size && b_i < b_size; a_i++, b_i++) {
		std::cout << a[a_i] << " vs " << b[b_i] << std::endl;
	}
	for (; a_i < a_size; a_i++) {
		std::cout << a[a_i] << " vs (none) " << std::endl;
	}
	for (; b_i < b_size; b_i++) {
		std::cout << "(none) vs " << b[b_i] << std::endl;
	}
}


/*	**************************************************************************	*/
/*	**************************************************************************	*/
/*							data types - input & outputs						*/
/*	**************************************************************************	*/
/*	**************************************************************************	*/

template <typename T>
SortMetrics bogusSort(T**array, array_size_t size) {
	SortMetrics result(0,0);
	result += SortingUtilities::randomizeArray(array, size);
	return result;
}

/*
 * given an algorithm & an ordering
 *	for each size min ... max	returns OneTestResult
 */
template <typename T>
OneTestResult<T>* testOneAlgorithm(	SortAlgorithms& algorithm,
									ArrayComposition& composition,
									InitialOrdering& ordering,
									SimpleRandomizer& randomizer,
									T *values,
									array_size_t size,
									num_repetitions_t num_repetitions)
{
	bool reset_generator = true;
	OneTestResult<T> *retval = new OneTestResult<T>(algorithm, composition, ordering, size, num_repetitions);

	SortingDataType<T> reference_data[size];
	SortingDataType<T> sorted_data[size];
	std::stringstream msg;

	//	_is_sorted will only get cleared the first time a sort fails
	retval->_failure_log->_diagnostics._is_sorted = true;
	SortingDataTypes::assignValues(reference_data, size, values, size);

	SortMetrics compares_and_moves;
	SortMetrics (*sort)(SortingDataType<T>**, array_size_t);

	switch (algorithm) {
	case SortAlgorithms::BUBBLE_SORT:
		sort = BubbleSort::sortPointersToObjects;
		break;
	case SortAlgorithms::DUTCH_FLAG_SORT:
		sort = DutchFlagSort::sortPointersToObjects;
		break;
	case SortAlgorithms::HEAP_SORT:
		sort = HeapSort::sortPointersToObjects;
		break;
	case SortAlgorithms::INSERTION_SORT:
		sort = InsertionSort::sortPointersToObjects;
		break;
	case SortAlgorithms::MERGE_SORT:
		sort = MergeSort::sortPointersToObjects;
		break;
	case SortAlgorithms::OPTIMIZED_QUICK_SORT:
		sort = OptimizedQuickSort::sortPointerstoObjects;
		break;
	case SortAlgorithms::QUICK_SORT:
		sort = QuickSort::sortPointerstoObjects;
		break;
	case SortAlgorithms::SELECTION_SORT:
		sort = SelectionSort::sortPointerstoObjects;
		break;
	case SortAlgorithms::BLOCK_SORT:
		sort = BlockSort::sortPointerstoObjects;
		break;
	case SortAlgorithms::INPLACE_MERGE:
		sort = InPlaceMerge::sortPointerstoObjects;
		break;

	case SortAlgorithms::RADIX_SORT:
	case SortAlgorithms::COUNTING_SORT:
	default:
		sort = bogusSort;
		break;
	}

	for (num_repetitions_t i = 0; i != num_repetitions; i++) {
		*sorted_data = *reference_data;
		disorganizeDataArray(*sorted_data, ordering, randomizer, false);
//		printSideBySide(*reference_data, *sorted_data);
		compares_and_moves = sort(sorted_data->_array, sorted_data->size());

//		printSideBySide(*reference_data, *sorted_data);
//		std::cout << "evaluating success of repetition " << i << std::endl;

		retval->_sort_metrics.compares += compares_and_moves.compares;
		retval->_sort_metrics.assignments += compares_and_moves.assignments;
		IsSortedResult *result = new IsSortedResult;
		result->_is_sorted =
			SortingUtilities::isSorted(sorted_data->_array, size,
									   compares_and_moves,
									   result->_mismatched_index_i,
									   result->_mismatched_index_j);

		if (!result->_is_sorted) {
			msg << "****************** FAILURE ON REPETITION #" << i << std::endl;
			msg << "[" << result->_mismatched_index_i << "] = " << *sorted_data->_array[result->_mismatched_index_i]
                << " is not less than [" << result->_mismatched_index_j << "] = " << *sorted_data->_array[result->_mismatched_index_j]
                << std::endl;
			std::cout << msg.str() << std::endl;
			retval->_failure_log = new SortFailureLog<T>();
			retval->_failure_log->_diagnostics = *result;
			retval->_failure_log->_input = reference_data;
			retval->_failure_log->_result = sorted_data;
			retval->_failure_log->_message = new std::string("Elements out of order");;
			retval->_messages->enqueue(msg.str());
			delete result;
			return retval;
		} else {
//			std::cout << "completed repetition " << i << std::endl;
		}
		delete result;
	}

	delete sorted_data;
	sorted_data = nullptr;
	delete reference_data;
	reference_data = nullptr;
	return retval;
}

//
//	OneTestResult<T> *result =
//		new OneTestResult<T>(algorithm, composition, ordering,
//							 size, num_repetitions);
//	return result;
//}


/*	**************************************************************************	*/
/*	**************************************************************************	*/
/*								utilities										*/
/*	**************************************************************************	*/
/*	**************************************************************************	*/

template <typename T>
void reverseArray(SortingDataType<T> *test_datay, array_size_t size);
template <typename T>
void minimallyRandomizeArray(SortingDataType<T> *test_data,
							 array_size_t num_outof_place);

int getNumSizes(array_size_t min, array_size_t max, array_size_t (*next)(array_size_t));

#endif /* SORTTEST_H_ */
