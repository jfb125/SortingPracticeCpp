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

#include "BubbleSort.h"
#include "DutchFlagSort.h"
#include "InsertionSort.h"
#include "SelectionSort.h"
#include "MergeSort.h"
#include "HeapSort.h"
#include "OptimizedQuickSort.h"
#include "QuickSort.h"

template <typename T>
ComparesAndMoves bogusSort(T**array, array_size_t size) {
	ComparesAndMoves result(0,0);
	result += SortingUtilities::randomizeArray(array, size);
	return result;
}

void	createDataArray(StudentDataArray& dst, ArrayComposition, StudentDataGenerator &, bool reset);
void	disorganizeDataArray(StudentDataArray&, InitialOrdering&, SimpleRandomizer&, bool restart);
IsSortedResult* isArrayOfPointersToObjectsSorted(StudentData **, array_size_t size);

/* ***********************************************************************	*/
/*				run a series of sort tests on one algorithm					*/
/* ***********************************************************************	*/

/*	create the reference_array
 *	repeat num_repetitions:
 *		copy the reference_array into a working copy
 *		disorganize the array
 *		sort the array
 */

OneTestResult* testOneAlgorithm(SortAlgorithms& algorithm,
								ArrayComposition& composition,
								InitialOrdering& ordering,
								SimpleRandomizer& randomizer,
								array_size_t size,
								num_repetitions_t num_repetitions) {

	bool reset_generator = true;
	OneTestResult *retval = new OneTestResult(algorithm, composition, ordering, size, num_repetitions);

	bool generator_is_initialized = true;
	StudentDataGenerator generator("AAAAAA", "Z",
								   StudentMajor::Undeclared_Major, 4.0,
								   Advance_Mode::Sequential_Disabled_Carry_In,
								   Advance_Mode::None,
								   Advance_Mode::None,
								   generator_is_initialized);
	StudentDataArray *reference_data = new StudentDataArray(size);
	StudentDataArray *sorted_data = new StudentDataArray(size);
	std::stringstream msg;

	//	_is_sorted will only get cleared the first time a sort fails
	retval->_failure_log->_diagnostics._is_sorted = true;
	createDataArray(*reference_data, composition, generator, reset_generator);

	ComparesAndMoves compares_and_moves;
	ComparesAndMoves (*sort)(StudentData**, array_size_t);

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

		retval->_sort_metrics._compares += compares_and_moves._compares;
		retval->_sort_metrics._moves += compares_and_moves._moves;
		IsSortedResult *result = new IsSortedResult;
		result->_is_sorted =
			SortingUtilities::isSorted(sorted_data->_array, size,
									   compares_and_moves,
									   result->_mismatched_index_i,
									   result->_mismatched_index_j);

		if (!result->_is_sorted) {
			msg << "****************** FAILURE ON REPETITION #" << i << std::endl;
			std::cout << msg.str() << std::endl;
			retval->_failure_log = new SortFailureLog();
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

/*	**************************************************************************	*/
/*	**************************************************************************	*/
/*								utilities										*/
/*	**************************************************************************	*/
/*	**************************************************************************	*/

void	createDataArray(StudentDataArray& array, ArrayComposition composition, StudentDataGenerator& generator, bool reset) {

	if (reset) {
		generator.reset();
	}

	StudentData* a_student;

	switch(composition._composition) {
	case ArrayCompositions::ALL_DISCRETE:
		for (array_size_t i = 0; i != array.size(); i++) {
			array[i] = generator.getNextStudent();
		}
		break;
	case ArrayCompositions::FEW_DISCRETE:
		{
			// determine how many of each discrete value should be assigned
			array_size_t discrete_divisor = composition._num_discrete_values;
			if (discrete_divisor == 0) {
				discrete_divisor = 1;
			}
			array_size_t count_of_each_value = array.size() / discrete_divisor;
			// use the ceiling of the result of the division
			if (array.size() % composition._num_discrete_values != 0)
				count_of_each_value++;
#if 0
			std::cout << " creating " << count_of_each_value
					  << " based upon " << composition._num_discrete_values
					  << " discrete values in an array of size " << array.size()
					  << std::endl;
#endif
			for (array_size_t array_index = 0;
							  array_index != array.size();
							  ) {
				a_student = generator.getNextStudent();
				for (array_size_t value_count = 0;
								  value_count != count_of_each_value;
								  value_count++) {
					array[array_index] = new StudentData(*a_student);
					if(++array_index == array.size())
						break;
				}
			}
		}
		break;
	case ArrayCompositions::FEW_DIFFERENT:
		{
			array_size_t i = 0;
			for (array_size_t diff_count = composition._num_different; diff_count != 0; diff_count--){
				array[i] = generator.getNextStudent();
				if (++i == array.size())
					break;
			}
			if (i != array.size()) {
				a_student = generator.getNextStudent();
				for (; i != array.size(); i++) {
					array[i] = new StudentData(*a_student);
				}
			}
		}
		break;
	case ArrayCompositions::ALL_SAME:
		{
			a_student = generator.getNextStudent();
			for (array_size_t i = 0; i != array.size(); i++) {
			array[i] = new StudentData(*a_student);
			}
		}
		break;
	default:
		std::cout << "createDataArry(array, composition) passed unrecognized composition" << std::endl;
		break;
	}
}

void	disorganizeDataArray(StudentDataArray &array, InitialOrdering& ordering, SimpleRandomizer &randomizer, bool restart) {

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
			StudentData *tmp;
			array_size_t r;
			for (array_size_t i  = 0; i != array.size(); i++) {
				r = randomizer.rand(i, array.size());
				tmp = array[i];
				array[i] = array[r];
				array[r] = tmp;
			}
		}
		break;
	case InitialOrderings::IN_REVERSE_ORDER:
		{
			array_size_t i = 0;
			array_size_t j = array.size()-1;
			StudentData *tmp;
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
			StudentData *tmp;
			for (array_size_t i = 0; i != ordering.num_out_of_place(); i++) {
				y = randomizer.rand(i, array.size());
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

IsSortedResult* isArrayOfPointersToObjectsSorted(StudentData **array, array_size_t size) {

	for (array_size_t i = 0, j = 1; j != size; i++, j++) {
		if (*(array[j]) < *(array[i])) {
			return new IsSortedResult(false, i, j);
		}
	}
	return new IsSortedResult(true, 0, 0);
}

int getNumSizes(array_size_t min, array_size_t max, array_size_t (*next)(array_size_t)) {

	if (max < min)
		return 0;

	int steps = 1;

	for (array_size_t i = min; i != max; i = next(i))
		steps++;

	return steps;
}

void reverseArray(StudentDataArray& array) {

}
void minimallyRandomizeArray(StudentData *, unsigned long num_outof_place) {

}

/*	**************************************************************************	*/
/*									output										*/
/*	**************************************************************************	*/

void	printSideBySide(StudentDataArray &a, StudentDataArray &b) {

	int current_width = std::cout.width();
	std::cout << std::setw(0) << a << "                : " << b << std::endl;

	for (array_size_t i = 0; i != a.size(); i++) {
		std::cout << *a[i] << ": " << *b[i] << std::endl;
	}
	std::cout << std::setw(current_width);
}

std::ostream& operator<<(std::ostream& out, const ComparesAndMoves& object) {

	out << "compares " << object._compares << ", swaps " << object._moves;
	return out;
}

ComparesAndMoves& operator+=(ComparesAndMoves& lhs, const ComparesAndMoves &rhs) {

	lhs._compares += rhs._compares;
	lhs._moves += rhs._moves;
	return lhs;
}
