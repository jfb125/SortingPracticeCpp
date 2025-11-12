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
#include "SortFailureLog.h"
#include "InitialOrdering.h"
#include "GenerateTestVectors.h"
#include "MessageList.h"
#include "OneTestResult.h"		// results of running n tests of a given array length
#include "ResultOutput.h"
#include "SimpleRandomizer.h"
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
#include "ProtectedQuickSort.h"
#include "QuickSort.h"


template <typename T>
void	printSideBySide(SortingDataType<T> *a, array_size_t a_size,
						SortingDataType<T> *b, array_size_t b_size) {

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

template <typename T>
void disorganizeDataArray(T* array,
						  array_size_t,
						  InitialOrdering& ordering,
						  SimpleRandomizer& randomizer,
						  bool restart);

/*	**************************************************************************	*/
/*	**************************************************************************	*/
/*							data types - input & outputs						*/
/*	**************************************************************************	*/
/*	**************************************************************************	*/

template <typename T>
void bogusSort(T* array, array_size_t size, SortMetrics *metrics = nullptr);

template <typename T>
void bogusSort(T* array, array_size_t size, SortMetrics *metrics) {
	SortingUtilities::randomizeArray(array, size, metrics);
	return;
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
									array_size_t array_size,
									num_repetitions_t num_repetitions)
{
	OStreamState ostream_state;
	bool debug_verbose = false;

	auto copy_array = [&array_size] (T *dst, T* src) {
		for (array_size_t i = 0; i != array_size; i++) {
			dst[i] = src[i];
		}
	};

	OneTestResult<T> *retval =
		new OneTestResult<T>(algorithm, composition, ordering, array_size, num_repetitions);
	retval->m_is_stable = true;

	PermutationGenerator<T> *permutation_generator = nullptr;
	if (composition.composition == ArrayCompositions::ALL_PERMUTATIONS) {
		factorial_t num_permutations = SortingUtilities::factorial(array_size);
		if (num_permutations > num_repetitions) {
			std::cout << "Test over all " << num_permutations
					  << " not performed because num_permutations is less than "
					  << num_repetitions
					  << " number of repetitions allowed " << std::endl;
			retval->m_ignore = true;
			//	avoid triggering an abort of successive tests
			retval->m_failure_log->m_diagnostics.is_sorted = true;
			return retval;
		}
		permutation_generator = new PermutationGenerator<T>(values, array_size);
	}

	T reference_data[array_size];
	copy_array(reference_data, values);

	T sorted_data[array_size];

	std::stringstream msg;

	//	_is_sorted will only get cleared the first time a sort fails
	retval->m_failure_log->m_diagnostics.is_sorted = true;

	void (*sort)(T*, array_size_t, SortMetrics*);

	switch (algorithm) {
	case SortAlgorithms::BUBBLE_SORT:
		sort = BubbleSort::sort;
		break;
	case SortAlgorithms::DUTCH_FLAG_SORT:
		sort = DutchFlagSort::sort;
		break;
	case SortAlgorithms::HEAP_SORT:
		sort = HeapSort::sort;
		break;
	case SortAlgorithms::INSERTION_SORT:
		sort = InsertionSort::sort;
		break;
	case SortAlgorithms::MERGE_SORT:
		sort = MergeSort::sort;
		break;
	case SortAlgorithms::PROTECTED_QUICK_SORT:
		sort = ProtectedQuickSort::sort;
		break;
	case SortAlgorithms::QUICK_SORT:
		sort = QuickSort::sort;
		break;
	case SortAlgorithms::SELECTION_SORT:
		sort = SelectionSort::sort;
		break;
	case SortAlgorithms::BLOCK_SORT:
		sort = BlockSort::sort;
		break;
	case SortAlgorithms::INPLACE_MERGE:
		sort = InPlaceMerge::sort;
		break;

	case SortAlgorithms::RADIX_SORT:
	case SortAlgorithms::COUNTING_SORT:
	default:
		sort = bogusSort;
		break;
	}

	T previous[array_size];
	bool permutations_done = false;
	for (num_repetitions_t i = 0;
						   i < num_repetitions && !permutations_done;
						   i++) {
		//	Generate a test vector
		if (composition.composition != ArrayCompositions::ALL_PERMUTATIONS) {
			copy_array(sorted_data, reference_data);
			disorganizeDataArray(sorted_data, array_size,
								 ordering, randomizer, false);
		} else {
			//	When testing all permutations, the reference_data vector
			//	changes each time (vs just getting disorganized each time)
			permutation_generator->next(reference_data);
			copy_array(sorted_data, reference_data);
			permutations_done = permutation_generator->is_done();
			if (permutations_done)
				retval->m_sort_metrics.num_repetitions = i+1;
		}
		SortingDataTypes::assignSequenceNumbers(sorted_data, array_size, 0);
		copy_array(previous, sorted_data);
		if (debug_verbose) {
			std::cout << std::setw(6) << i << ": "
					  << SortingUtilities::arrayElementsToString(sorted_data, array_size)
					  << std::endl;
		}
		SortMetrics compares_and_moves(0,0);
//		printSideBySide(*reference_data, *sorted_data);
		sort(sorted_data, array_size, &compares_and_moves);
//		printSideBySide(*reference_data, *sorted_data);
//		std::cout << "evaluating success of repetition " << i << std::endl;

		retval->m_sort_metrics 	+= compares_and_moves;
		//	if every sort up to this point has been stable,
		//	  see if this sort was stable
		if (retval->m_is_stable) {
			retval->m_is_stable	=
					SortingUtilities::isStable(sorted_data, array_size);
		}

		IsSortedResult *result = new IsSortedResult;
		result->is_sorted =
			SortingUtilities::isSorted(sorted_data, array_size,
									   result->mismatched_index_i,
									   result->mismatched_index_j);

//			SortingUtilities::isStable(sorted_data, array_size);
		if (!result->is_sorted) {
			msg << "****************** FAILURE ON REPETITION #" << i << std::endl;
			msg << "[" << result->mismatched_index_i << "] = "
//				<< sorted_data[result->_mismatched_index_i]
                << " is not less than [" << result->mismatched_index_j << "] = "
//				<< *sorted_data[result->_mismatched_index_j]
                << std::endl;
			std::cout << msg.str() << std::endl;
			retval->m_failure_log = new SortFailureLog<T>();
			retval->m_failure_log->m_diagnostics = *result;
			retval->m_failure_log->copy_input(reference_data, array_size);
			retval->m_failure_log->copy_result(sorted_data, array_size);
			retval->m_failure_log->_message = new std::string("Elements out of order");;
			retval->m_messages->enqueue(msg.str());
			delete result;
			goto SORT_TEST_ONE_ALGORITHM_RETURN_LABEL;
			return retval;
		} else {
//			std::cout << "completed repetition " << i << std::endl;
		}
		delete result;
	}
SORT_TEST_ONE_ALGORITHM_RETURN_LABEL:
	if (permutation_generator) {
		delete permutation_generator;
		permutation_generator = nullptr;
	}
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
void reverseArray(T *test_datay, array_size_t size);
template <typename T>
void minimallyRandomizeArray(T *test_data,
							 array_size_t num_outof_place);

int getNumSizes(array_size_t min, array_size_t max, array_size_t (*next)(array_size_t));

template <typename T>
void	disorganizeDataArray(T *array,
							 array_size_t size,
							 InitialOrdering& ordering,
							 SimpleRandomizer &randomizer,
							 bool restart) {
	if (restart) {
		randomizer.restart();
	}

	switch(ordering.ordering()) {
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
			T tmp;
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
		{
			array_size_t y = 0;
			T tmp;
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

#endif /* SORTTEST_H_ */
