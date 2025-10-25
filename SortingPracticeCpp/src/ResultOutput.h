/*
 * ResultOutputParameters.h
 *
 *  Created on: Jun 21, 2025
 *      Author: joe
 */

#ifndef RESULTOUTPUTPARAMETERS_H_
#define RESULTOUTPUTPARAMETERS_H_

#include <iostream>
#include <iomanip>
#include <limits>
#include <cstring>	// for strlen()

#include "SortTest.h"

#define PRINT_TEST_RESULT_TABLE_HEADER "===================================="

constexpr char test_result_table_header[] = PRINT_TEST_RESULT_TABLE_HEADER;
constexpr char average_compares_string[] = "avg cmp";
constexpr char average_moves_string[] = "avg mov";
constexpr char colon_separator[] =  ": ";
constexpr char space_separator[] = "  ";
constexpr char slash_separator[] = " / ";
constexpr int compare_moves_fudge_factor = 4;
constexpr int average_compares_strlen = 7 + compare_moves_fudge_factor;
constexpr int average_moves_strlen = 7 + compare_moves_fudge_factor;
constexpr int comma_separator_strlen = 2;
constexpr int colon_separator_strlen = 2;
constexpr int space_separator_strlen = 2;
constexpr int slash_separator_strlen = 3;


/*	**************************************************************	*/
/*		class that contains all of the array size's data for table	*/
/*	**************************************************************	*/

class ArraySizeData {
public:
	int num_sizes;
	array_size_t min_size;
	array_size_t max_size;
	array_size_t *sizes;

	ArraySizeData() {
		num_sizes = 0;
		min_size = MAX_ARRAY_SIZE_T;
		max_size = MIN_ARRAY_SIZE_T;
		sizes = nullptr;
	}

	std::string str() {
		std::stringstream retval;
		retval << "size list contains " << num_sizes << " sizes "
			   << " from " << min_size
			   << " to "  << max_size
			   << ": ";
		if (sizes) {
			for (int i = 0; i != num_sizes-1; i++) {
				retval << sizes[i] << ", ";
			}
			retval << sizes[num_sizes-1];
		} else {
			retval << "<nullptr>";
		}
		return retval.str();
	}
};


/*	**********************************************************************	*/
/*								forward declarations						*/
/*	**********************************************************************	*/

void printRowStart(SortAlgorithms &algorithm, ArrayComposition &composition, InitialOrdering &ordering,
					int algorithm_strlen, int composition_strlen, int ordering_strlen);


/*	**********************************************************************	*/
/*							templated function declarations					*/
/*	**********************************************************************	*/

//	gets the ArraySizeData from 'results'  This is used as the table header
template <typename T>
void getArraySizeData(ArraySizeData &dst,
					  OneTestResult<T>** results, int num_test_results);

//	gets the width of the largest array size in the 'results'
template <typename T>
int getMaxDigitsArraySize(OneTestResult<T>** result, int num_tests);

//	gets the width of the largest number of assignments in the 'results'
template <typename T>
int getMaxDigitsAssignments(OneTestResult<T>** result, int num_tests);

//	gets the width of the largest number of compares in the 'results'
template <typename T>
int getMaxDigitsCompares(OneTestResult<T>** result, int num_tests);

//	gets the length of the largest algorithm's string amongst 'results'
template <typename T>
int getstrlenAlgorithm(OneTestResult<T>** results, int num_tests);

//	gets the length of the largest composition's string amongst 'results'
template <typename T>
int getstrlenComposition(OneTestResult<T>** result, int num_tests);

//	gets the length of the largest ordering's string amongst 'results'
template <typename T>
int getstrlenOrdering(OneTestResult<T>** result, int num_tests);

//	Is used to aid in sortResultArray() into the order they will go in
//	the output table.  The order of priority is alg->comp->order->size
template <typename T>
bool isLess_AlgorithmCompostionOrderingSize(OneTestResult<T>* u, OneTestResult<T>* v);

//	Is used to aid in sortResultArray() into the order they will go in
//	the output table.  The order of priority is comp->order->alg->size
template <typename T>
bool isLess_CompostionOrderingAlgorithmSize(OneTestResult<T>* u, OneTestResult<T>* v);

//	verifies that this OneTestResult matches algorithm & composition & ordering
//	this is used to determine when to go to a new line in the result table
template <typename T>
bool isSameLine(OneTestResult<T>* result,
				SortAlgorithms &algorithm,
				ArrayComposition &composition,
				InitialOrdering &ordering);

//	Prints the results into a table with the column headers being array sizes,
//	each row represents an algorithm, and the table's contents are the metrics
template <typename T>
void printRowsAlgorithm_ColumnsSize_CellsAverages(OneTestResult<T>** results,
												  int num_test_results,
												  bool (*isLess)(OneTestResult<T>*, OneTestResult<T>*));

//	The function to call to get a table of test results outputted to the screen
template <typename T>
void printTestResults(OneTestResult<T> **results, int num_test_results);

//	This sorts the array of pointers to individual results using 'isULessThanV
//	into the order the results will appear on the screen
template <typename T>
void sortResultsArray(OneTestResult<T>** results, int num_tests,
					  bool (*isULessThanV)(OneTestResult<T>* u, OneTestResult<T>* v));

//	Dumps the parameters of one result without dumping the sort_metrics
//	'i', if >= 0, is printed at the start of the line
template <typename T>
void terseDump(OneTestResult<T>* result, int i);

//	Dumps the parameters of each result without dumping the sort_metrics
template <typename T>
void terseDump(OneTestResult<T>** results, int num_test_results);


/*	**********************************************************************	*/
/*							templated function definitions					*/
/*	**********************************************************************	*/


/*				getstrlenAlgorithm()				*/

template <typename T>
int getstrlenAlgorithm(OneTestResult<T>** results, int num_tests) {

	int max_len = 0;

	for (int i = 0; i != num_tests; i++) {
		std::string algstr = toString(results[i]->m_algorithm);
		int len = strlen(algstr.c_str());
		if (len > max_len) {
			max_len = len;
		}
	}
	return max_len;
}


/*				getstrlenComposition()				*/

template <typename T>
int getstrlenComposition(OneTestResult<T>** result, int num_tests) {

	int max_len = 0;

	for (int i = 0; i != num_tests; i++) {
		int len = result[i]->m_composition.str().length();
		if (len > max_len) {
			max_len = len;
		}
	}
	return max_len;
}


/*				getstrlenOrdering()				*/

template <typename T>
int getstrlenOrdering(OneTestResult<T>** result, int num_tests) {

	int max_len = 0;

	for (int i = 0; i != num_tests; i++) {
		int len = result[i]->m_ordering.str().length();
		if (len > max_len) {
			max_len = len;
		}
	}
	return max_len;
}


/*				getMaxDigitsCompares()			*/

template <typename T>
int getMaxDigitsCompares(OneTestResult<T>** result, int num_tests) {

	int max_digits = 1;

	// determine the largest number of compares
	total_compares_t max_compares = 0;
	for (int i = 0; i != num_tests; i++) {
		// trying to improve readability
		total_compares_t this_compares = result[i]->m_sort_metrics.compares;
		if (this_compares > max_compares) {
			max_compares = this_compares;
		}
	}

	// determine the number of decimal digits
	//	in the largest number of compares
	while (max_compares != 0) {
		max_digits++;
		max_compares /= 10;
	}

	return max_digits;
}


/*					getMaxDigitsAssignments()			*/

template <typename T>
int getMaxDigitsAssignments(OneTestResult<T>** result, int num_tests) {

	int max_digits = 1;

	// determine the largest number of compares
	total_moves_t max_moves = 0;
	for (int i = 0; i != num_tests; i++) {
		// trying to improve readability
		total_moves_t this_moves = result[i]->m_sort_metrics.assignments;
		if (this_moves > max_moves) {
			max_moves = this_moves;
		}
	}

	// determine the number of decimal digits
	//	in the largest number of compares
	while (max_moves != 0) {
		max_digits++;
		max_moves /= 10;
	}

	return max_digits;
}


/*					getMaxDigitsArraySize()			*/

template <typename T>
int getMaxDigitsArraySize(OneTestResult<T>** result, int num_tests) {

	int max_array_digits = 1;

	// determine largest array size
	array_size_t max_array_size = 0;
	for (int i = 0; i != num_tests; i++) {
		// improve readability
		array_size_t this_size = (*result)[i]._size;
		if (this_size > max_array_size) {
			max_array_size = this_size;
		}
	}

	// determine num of decimal digits of max array size

	while (max_array_size) {
		max_array_digits++;
		max_array_size /= 10;
	}
	return max_array_digits;
}


/*						getArraySizeData()		*/

template <typename T>
void getArraySizeData(ArraySizeData &dst,
				  	 OneTestResult<T>** results, int num_test_results) {

	// linked list is used to identify unique values of '_size' in results
	struct ArraySizeNode {
		array_size_t m_size;
		ArraySizeNode *m_next;
		ArraySizeNode *m_prev;
		ArraySizeNode(array_size_t size) {
			m_size = size;
			m_next = nullptr;
			m_prev = nullptr;
		}
	};

	dst.num_sizes 	= 0;
	dst.min_size 	= MAX_ARRAY_SIZE_T;
	dst.max_size 	= MIN_ARRAY_SIZE_T;
	dst.sizes 		= nullptr;

	ArraySizeNode *head = nullptr;
	ArraySizeNode *tail = nullptr;
	ArraySizeNode *seeker = nullptr;

	for (int i = 0; i != num_test_results; i++) {
		seeker = head;
		array_size_t this_size = results[i]->m_size;
		// determine if this_size represents a new min or max
		if (this_size < dst.min_size) {
			dst.min_size = this_size;
		}
		if (this_size > dst.max_size) {
			dst.max_size = this_size;
		}
		// try to find 'this_size' in the list of known sizes
		if (seeker == nullptr) {
			// if the list is empty, enter this_size into head
			head = new ArraySizeNode(this_size);
			tail = head;
			dst.num_sizes++;
		} else {
			while (seeker != nullptr) {
				// if the value is found in the list,
				//	break out to the for loop to look at next result
				if (seeker->m_size == this_size) {
					break;
				}
				// the current node does not contain the value
				if (seeker->m_next == nullptr) {
					// the current node is the last value
					// create a new node with unique this_value
					seeker->m_next = new ArraySizeNode(this_size);
					seeker->m_next->m_prev = seeker;
					tail = seeker->m_next;
					//  increment the number of unique values
					dst.num_sizes++;
				}	// if seeker->m_next == nullptr
				seeker = seeker->m_next;
			}	// while seeker != nullptr
		}	// else list is not empty
	}	// for i <= num_sizes

	// copy the unique size values that were stored in the linked list
	dst.sizes = new array_size_t[dst.num_sizes];
	seeker = head;
	int size_index = 0;
	while (seeker) {
		dst.sizes[size_index++] = seeker->m_size;
		seeker = seeker->m_next;
	}

	// sort the unique size values that are sorted in the linked list
	for (int last_unsorted = dst.num_sizes-1; last_unsorted != 0; last_unsorted--) {
		bool was_swap = false;
		for (int j = 0; j < last_unsorted; j++) {
			if (dst.sizes[j] > dst.sizes[j+1]) {
				array_size_t tmp = dst.sizes[j];
				dst.sizes[j] = dst.sizes[j+1];
				dst.sizes[j+1] = tmp;
				was_swap = true;
			}
		}
		if (!was_swap)
			break;
	}

	// free the list
	while (tail->m_prev) {
		tail = tail->m_prev;
		delete tail->m_next;
		tail->m_next = nullptr;
	}
	delete tail;
	tail = nullptr;
}


/*			isLess_AlgorithmCompostionOrderingSize()		*/

template <typename T>
bool isLess_AlgorithmCompostionOrderingSize(OneTestResult<T>* u, OneTestResult<T>* v) {
	//	compare algorithm
	if (u->m_algorithm < v->m_algorithm)	return true;
	if (u->m_algorithm > v->m_algorithm)	return false;
	//	compare composition
	if (u->m_composition < v->m_composition) return true;
	if (u->m_composition > v->m_composition) return false;
	//	compare orderings
	if (u->m_ordering < v->m_ordering)	return true;
	if (u->m_ordering > v->m_ordering)	return false;
	//	compares sizes
	if (u->m_size < v->m_size)	return true;
	if (u->m_size > v->m_size)	return false;
	//	the test result parameters are identical
	return false;
}


/*				isLess_CompostionOrderingAlgorithmSize()		*/

template <typename T>
bool isLess_CompostionOrderingAlgorithmSize(OneTestResult<T>* u, OneTestResult<T>* v) {
	//	compare composition
	if (u->m_composition < v->m_composition) return true;
	if (u->m_composition > v->m_composition) return false;
	//	compare orderings
	if (u->m_ordering < v->m_ordering)	return true;
	if (u->m_ordering > v->m_ordering)	return false;
	//	compare algorithm
	if (u->m_algorithm < v->m_algorithm)	return true;
	if (u->m_algorithm > v->m_algorithm)	return false;
	//	compares sizes
	if (u->m_size < v->m_size)	return true;
	if (u->m_size > v->m_size)	return false;
	//	the test result parameters are identical
	return false;
}


/*				isSameLine()					*/

template <typename T>
bool isSameLine(OneTestResult<T>* result,
				SortAlgorithms &algorithm,
				ArrayComposition &composition,
				InitialOrdering &ordering)
{
	if (result->m_algorithm != algorithm)		return false;
	if (result->m_composition != composition) 	return false;
	if (result->m_ordering != ordering)			return false;
	return true;
}


/*				printRowsAlgorithm_ColumnsSize_CellsAverages()		*/

template <typename T>
void printRowsAlgorithm_ColumnsSize_CellsAverages(OneTestResult<T>** results,
												  int num_test_results,
												  bool (*isLess)(OneTestResult<T>*, OneTestResult<T>*)) {

	OStreamState ostream_state;	// restores ostream state in destructor

	sortResultsArray(results, num_test_results, isLess);

	ArraySizeData size_data;
	getArraySizeData(size_data, results, num_test_results);

	int max_algorithm_strlen 	= getstrlenAlgorithm(results, num_test_results);
	int max_composition_strlen 	= getstrlenComposition(results, num_test_results);
	int max_ordering_strlen 	= getstrlenOrdering(results, num_test_results);
	int max_digits_compares 	= getMaxDigitsCompares(results, num_test_results);
	int max_digits_moves 		= getMaxDigitsAssignments(results, num_test_results);
	int compares_strlen	= max_digits_compares;
	int moves_strlen	= max_digits_moves;

	if (compares_strlen < average_compares_strlen) {
		compares_strlen = average_compares_strlen;
	}
	if (moves_strlen < average_moves_strlen) {
		moves_strlen = average_moves_strlen;
	}

	SortAlgorithms current_sort 		= SortAlgorithms::SORT_ALGORITHMS_COUNT;
	ArrayComposition current_composition= ArrayCompositions::ARRAY_COMPOSITIONS_COUNT;
	InitialOrdering current_ordering 	= InitialOrderings::INITIAL_ORDERINGS_COUNT;

	int array_size_header_width =
			max_algorithm_strlen + max_composition_strlen + max_ordering_strlen +
			comma_separator_strlen + comma_separator_strlen + colon_separator_strlen;

	std::cout << std::setw(array_size_header_width) << std::right << "Array sizes: ";
	for (int i = 0; i != size_data.num_sizes; i++) {
		std::cout << std::setw(compares_strlen + slash_separator_strlen) << size_data.sizes[i];
		std::cout << std::setw(moves_strlen) << "" << space_separator;
	}
	std::cout << std::endl;

	std::stringstream rep_str;
	rep_str << "number of repetitions: " << results[0]->m_sort_metrics.num_repetitions;
	std::cout << std::setw(array_size_header_width) << std::left << rep_str.str();
	for (int i = 0; i != size_data.num_sizes; i++) {
		std::cout << std::setw(compares_strlen) << std::right << average_compares_string
				  << std::setw(0) << slash_separator
				  << std::setw(moves_strlen) << std::left << average_moves_string
				  << space_separator;
	}
	std::cout << std::endl;

	SortAlgorithms	first_sort_in_group = results[0]->m_algorithm;
	for (int i = 0; i != num_test_results; i++) {
		if(!isSameLine(results[i], current_sort, current_composition, current_ordering)) {
			current_sort = results[i]->m_algorithm;
			current_composition = results[i]->m_composition;
			current_ordering = results[i]->m_ordering;
			if (i != 0) {
				std::cout << std::endl;
				if (current_sort == first_sort_in_group) {
					std::cout << std::endl;
				}
			}
			printRowStart(current_sort, current_composition, current_ordering,
						  max_algorithm_strlen, max_composition_strlen, max_ordering_strlen);
		}
		std::cout << std::setw(compares_strlen) << std::right
				  << results[i]->m_sort_metrics.comparesStr()
				  << std::setw(0) << slash_separator
				  << std::setw(moves_strlen) << std::right
				  << results[i]->m_sort_metrics.movesStr()
				  << space_separator;
		// HACK - get rid of '-2' and "  "
	}
	std::cout << std::endl << std::endl;

	if (size_data.sizes) {
		delete size_data.sizes;
		size_data.sizes = nullptr;
	}
}


/*				printTestResults()					*/

template <typename T>
void printTestResults(OneTestResult<T> **results, int num_test_results) {

	std::cout << test_result_table_header << std::endl
			  << "Sort Test Results" << std::endl
			  << test_result_table_header << std::endl;
	bool (*isLess)(OneTestResult<T>* u, OneTestResult<T>*v);
	isLess = isLess_CompostionOrderingAlgorithmSize;
	printRowsAlgorithm_ColumnsSize_CellsAverages(results, num_test_results, isLess);
}


/*
 *	bubble sort with  !was_swap  optimization
 */
template <typename T>
void sortResultsArray(OneTestResult<T>** results, int num_tests,
					  bool (*isULessThanV)(OneTestResult<T>* u, OneTestResult<T>* v)) {

	if (results == nullptr)
		return;
	if (num_tests <= 1)
		return;

	for (int closest_unordered = 0;
		 closest_unordered != num_tests;
		 closest_unordered++) {
		bool was_swap = false;
		for (int bubble_up = num_tests-1;
			 bubble_up > closest_unordered; bubble_up--) {
			// if the element earlier in the array is not less than this element
			if (!isULessThanV(results[bubble_up-1], results[bubble_up])) {
				OneTestResult<T> *tmp 	= results[bubble_up];
				results[bubble_up] 		= results[bubble_up-1];
				results[bubble_up-1] 	= tmp;
				was_swap = true;
			}
		}
		if (!was_swap)
			break;
	}
}


/*				terseDump()					*/

template <typename T>
void terseDump(OneTestResult<T>* results, int i) {
	if (i >= 0) {
		std::cout << std::setw(2) << i;
	}
	std::cout << results->m_algorithm << ", "
			  << results->m_composition << ", "
			  << results->m_ordering << ", "
			  << results->m_size
			  << std::endl;
}

template <typename T>
void terseDump(OneTestResult<T>** results, int num_test_results) {

	for (int i = 0; i != num_test_results-1; i++) {
		terseDump(results[i], i);
	}
	//	don't add an std::endl after the final result
	terseDump(results[num_test_results-1], num_test_results-1);
}

#endif /* RESULTOUTPUTPARAMETERS_H_ */
