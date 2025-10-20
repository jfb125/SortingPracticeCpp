/*
 * Sort_Test.cpp
 *
 *  Created on: Apr 15, 2025
 *      Author: joe
 */

#include <cstdlib>
#include <ctime>

#include "SortTest.h"
#include "SortingUtilities.h"
#include "SortingDataTypes.h"

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
void	disorganizeDataArray(SortingDataType<T> *,
							 InitialOrdering&,
							 SimpleRandomizer&,
							 bool restart);

/* ***********************************************************************	*/
/*				run a series of sort tests on one algorithm					*/
/* ***********************************************************************	*/

/*	create the reference_array
 *	repeat num_repetitions:
 *		copy the reference_array into a working copy
 *		disorganize the array
 *		sort the array
 */
#if 0
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
#endif
template <typename T>
void	disorganizeDataArray(SortingDataType<T> *array,
							 array_size_t size,
							 InitialOrdering& ordering,
							 SimpleRandomizer &randomizer,
							 bool restart) {
	if (restart) {
		randomizer.restart();
	}
	if (!isValid(ordering.order())) {
		std::cout << "disorganizeDataArray(array, ordering, randomiser, restart) passed invalid ordering";
		return;
	}

	switch(ordering.order()) {
	case InitialOrderings::IN_RANDOM_ORDER:
		{
			T tmp;
			array_size_t r;
			for (array_size_t i  = 0; i != size; i++) {
				r = randomizer.rand(i, size);
				tmp = array[i];
				array[i] = array[r];
				array[r] = tmp;
			}
		}
		break;
	case InitialOrderings::IN_REVERSE_ORDER:
		{
			array_size_t i = 0;
			array_size_t j = size-1;
			SortingDataType<T> tmp;
			while (i < j) {
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
				i++;
				j--;
			}
		}
		break;
	case InitialOrderings::FEW_CHANGES:
		if (ordering.num_out_of_place_is_initialized()) {
			array_size_t y = 0;
			SortingDataType<T> tmp;
			for (array_size_t i = 0; i != ordering.num_out_of_place(); i++) {
				y = randomizer.rand(i, size);
				tmp = array[i];
				array[i] = array[y];
				array[y] = tmp;
			}
		}
		break;
	case InitialOrderings::NO_CHANGES:
	default:
		break;
	}
}

int getNumSizes(array_size_t min, array_size_t max, array_size_t (*next)(array_size_t)) {

	if (max < min)
		return 0;

	int steps = 1;

	for (array_size_t i = min; i != max; i = next(i))
		steps++;

	return steps;
}
