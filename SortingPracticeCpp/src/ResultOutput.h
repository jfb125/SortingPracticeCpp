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

constexpr char test_result_table_header[] 			= PRINT_TEST_RESULT_TABLE_HEADER;
constexpr char average_compares_string[]  			= "avg cmp";	// seven chars long
constexpr char average_assignments_string[] 		= "avg asgn";	// eight chars long
constexpr char stability_string[]					= "stbl";
constexpr char is_stable_string[]					= " Y  ";
constexpr char is_not_stable_string[]				= " n  ";
constexpr char is_not_applicable_stability_string[]	= "n/a ";
constexpr char colon_separator[] 					= ": ";
constexpr char comma_separator[]					= ", ";
constexpr char space_separator[] 					= "  ";
constexpr char slash_separator[] 					= " / ";
constexpr int comma_separator_strlen 				= 2;
constexpr int colon_separator_strlen 				= 2;
constexpr int space_separator_strlen 				= 2;
constexpr int slash_separator_strlen 				= 3;
constexpr int average_compares_strlen 	 			= 7;
constexpr int average_assignments_strlen 			= 8;
constexpr int stability_string_strlen				= 4;

/*	**************************************************************************	*/
/*			Information about how the output table should be structured			*/
/*	**************************************************************************	*/

enum class ResultTableElements {
	ALGORITHM,
	COMPOSITION,
	ORDERING,
	SIZE
};

#define DEFAULT_RESULT_TABLE_FIRST 			ResultTableElements::ALGORITHM
#define DEFAULT_RESULT_TABLE_SECOND			ResultTableElements::COMPOSITION
#define DEFAULT_RESULT_TABLE_THIRD			ResultTableElements::ORDERING
#define	DEFAULT_RESULT_TABLE_END_OF_BLOCK	ResultTableElements::ORDERING

class ResultTableOrdering {
	ResultTableElements line_preamble_first;
	ResultTableElements line_preamble_second;
	ResultTableElements line_preamble_third;
	ResultTableElements m_end_of_block;

public:
	ResultTableOrdering(ResultTableElements _first		= DEFAULT_RESULT_TABLE_FIRST,
						ResultTableElements _second		= DEFAULT_RESULT_TABLE_SECOND,
						ResultTableElements _third		= DEFAULT_RESULT_TABLE_THIRD,
						ResultTableElements _linefeed	= DEFAULT_RESULT_TABLE_END_OF_BLOCK)
		:	line_preamble_first(_first),
			line_preamble_second(_second),
			line_preamble_third(_third),
			m_end_of_block(_linefeed) {}
	~ResultTableOrdering() {}

	ResultTableElements first(void) const 	{ return line_preamble_first;	}
	ResultTableElements second(void) const	{ return line_preamble_second;	}
	ResultTableElements third(void) const	{ return line_preamble_third;	}
	ResultTableElements block_end(void) const	{ return m_end_of_block;	}

	ResultTableOrdering& first(ResultTableElements arg) {
		line_preamble_first = arg;
		return *this;	}
	ResultTableOrdering& second(ResultTableElements arg) {
		line_preamble_second = arg;
		return *this;	}
	ResultTableOrdering& third(ResultTableElements arg)	{
		line_preamble_third = arg;
		return *this;	}
	ResultTableOrdering& block_end(ResultTableElements arg)	{
		m_end_of_block = arg;
		return *this;	}
};

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
template <typename T>
using CompareFunction = bool (*)(OneTestResult<T>*u, OneTestResult<T>*v);

template <typename T>
CompareFunction<T> assignCompareFunction(ResultTableOrdering& structure);

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
bool isLess_AlgorithmCompositionOrderingSize(OneTestResult<T>* u, OneTestResult<T>* v);

//	Is used to aid in sortResultArray() into the order they will go in
//	the output table.  The order of priority is alg->comp->order->size
template <typename T>
bool isLess_AlgorithmOrderingCompositionSize(OneTestResult<T>* u, OneTestResult<T>* v);

//	Is used to aid in sortResultArray() into the order they will go in
//	the output table.  The order of priority is alg->order->comp->size
template <typename T>
bool isLess_CompositionOrderingAlgorithmSize(OneTestResult<T>* u, OneTestResult<T>* v);

//	Is used to aid in sortResultArray() into the order they will go in
//	the output table.  The order of priority is comp->order->alg->size
template <typename T>
bool isLess_CompositionAlgorithmOrderingSize(OneTestResult<T>* u, OneTestResult<T>* v);

//	Is used to aid in sortResultArray() into the order they will go in
//	the output table.  The order of priority is order->alg->comp->size
template <typename T>
bool isLess_OrderingAlgorithmCompositionSize(OneTestResult<T>* u, OneTestResult<T>* v);

//	Is used to aid in sortResultArray() into the order they will go in
//	the output table.  The order of priority is order->comp->alg->size
template <typename T>
bool isLess_OrderingCompositionAlgorithmSize(OneTestResult<T>* u, OneTestResult<T>* v);

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
void printRowPreamble_ColumnsSize_CellsAverages(OneTestResult<T>** results,
												  int num_test_results,
												  ResultTableOrdering &structure);

//	The function to call to get a table of test results outputted to the screen
template <typename T>
void printTestResults(OneTestResult<T> **results, int num_test_results);

//	Prints out the start of the line
std::string rowPreambleToString(SortAlgorithms &algorithm,
							 	ArrayComposition &composition,
								InitialOrdering &ordering,
								int algorithm_strlen,
								int composition_strlen,
								int ordering_strlen,
								ResultTableOrdering& structure);

//	This sorts the array of pointers to individual results using 'isULessThanV
//	into the order the results will appear on the screen
template <typename T>
void sortResultsArray(OneTestResult<T>** results, int num_tests,
					  CompareFunction<T> isULessThanV);

std::string stabilityToString(ArrayCompositions composition, bool stable);

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

template <typename T>
CompareFunction<T> assignCompareFunction(ResultTableOrdering& structure)
{
	switch (structure.first()) {
	case ResultTableElements::ALGORITHM:
	default:
		if (structure.second() == ResultTableElements::COMPOSITION) {
			return isLess_AlgorithmCompositionOrderingSize<T>;
		} else {
			return isLess_AlgorithmOrderingCompositionSize<T>;
		}
		break;
	case ResultTableElements::COMPOSITION:
		if (structure.second() == ResultTableElements::ALGORITHM) {
			return isLess_CompositionAlgorithmOrderingSize<T>;
		} else {
			return isLess_CompositionOrderingAlgorithmSize<T>;
		}
		break;
	case ResultTableElements::ORDERING:
		if (structure.second() == ResultTableElements::COMPOSITION) {
			return isLess_OrderingCompositionAlgorithmSize<T>;
		} else {
			return isLess_OrderingAlgorithmCompositionSize<T>;
		}
		break;
	}
}

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
		int len = result[i]->m_composition.to_string().length();
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
		int len = result[i]->m_ordering.to_string().length();
		if (len > max_len) {
			max_len = len;
		}
	}
	return max_len;
}


/*				getMaxDigitsRepetitions()			*/

template <typename T>
int getMaxDigitsRepetitions(OneTestResult<T>** result, int num_tests) {

	int max_digits = 0;	// there will be at least one digit

	// determine the largest number of compares
	num_repetitions_t max_repetitions = 0;
	for (int i = 0; i != num_tests; i++) {
		// trying to improve readability
		num_repetitions_t this_repetitions =
				result[i]->m_sort_metrics.num_repetitions;
		if (this_repetitions > max_repetitions) {
			max_repetitions = this_repetitions;
		}
	}

	// determine the number of decimal digits
	//	in the largest number of compares
	while (max_repetitions != 0) {
		max_digits++;
		max_repetitions /= 10;
	}

	return max_digits;
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

template <typename T>
bool isLess_AlgorithmCompositionOrderingSize(OneTestResult<T>* u, OneTestResult<T>* v) {
	//	compare algorithm
	if (u->m_algorithm < v->m_algorithm)	return true;
	if (u->m_algorithm > v->m_algorithm)	return false;
	//	compare composition
	if (u->m_composition < v->m_composition) return true;
	if (u->m_composition > v->m_composition) return false;
	//	compare orderings
	if (u->m_ordering < v->m_ordering)		return true;
	if (u->m_ordering > v->m_ordering)		return false;
	//	compares sizes
	if (u->m_size < v->m_size)				return true;
	if (u->m_size > v->m_size)				return false;
	//	the test result parameters are identical
	return false;
}

template <typename T>
bool isLess_AlgorithmOrderingCompositionSize(OneTestResult<T>* u, OneTestResult<T>* v) {
	//	compare algorithm
	if (u->m_algorithm < v->m_algorithm)	return true;
	if (u->m_algorithm > v->m_algorithm)	return false;
	//	compare orderings
	if (u->m_ordering < v->m_ordering)		return true;
	if (u->m_ordering > v->m_ordering)		return false;
	//	compare composition
	if (u->m_composition < v->m_composition) return true;
	if (u->m_composition > v->m_composition) return false;
	//	compares sizes
	if (u->m_size < v->m_size)				return true;
	if (u->m_size > v->m_size)				return false;
	return false;
}

template <typename T>
bool isLess_CompositionAlgorithmOrderingSize(OneTestResult<T>* u, OneTestResult<T>* v) {
	//	compare composition
	if (u->m_composition < v->m_composition) return true;
	if (u->m_composition > v->m_composition) return false;
	//	compare algorithm
	if (u->m_algorithm < v->m_algorithm)	return true;
	if (u->m_algorithm > v->m_algorithm)	return false;
	//	compare orderings
	if (u->m_ordering < v->m_ordering)		return true;
	if (u->m_ordering > v->m_ordering)		return false;
	//	compares sizes
	if (u->m_size < v->m_size)				return true;
	if (u->m_size > v->m_size)				return false;
	return false;
}

template <typename T>
bool isLess_CompositionOrderingAlgorithmSize(OneTestResult<T>* u, OneTestResult<T>* v) {
	//	compare composition
	if (u->m_composition < v->m_composition) return true;
	if (u->m_composition > v->m_composition) return false;
	//	compare orderings
	if (u->m_ordering < v->m_ordering)		return true;
	if (u->m_ordering > v->m_ordering)		return false;
	//	compare algorithm
	if (u->m_algorithm < v->m_algorithm)	return true;
	if (u->m_algorithm > v->m_algorithm)	return false;
	//	compares sizes
	if (u->m_size < v->m_size)				return true;
	if (u->m_size > v->m_size)				return false;
	return false;
}

template <typename T>
bool isLess_OrderingAlgorithmCompositionSize(OneTestResult<T>* u, OneTestResult<T>* v) {
	//	compare orderings
	if (u->m_ordering < v->m_ordering)		return true;
	if (u->m_ordering > v->m_ordering)		return false;
	//	compare algorithm
	if (u->m_algorithm < v->m_algorithm)	return true;
	if (u->m_algorithm > v->m_algorithm)	return false;
	//	compare composition
	if (u->m_composition < v->m_composition) return true;
	if (u->m_composition > v->m_composition) return false;
	//	compares sizes
	if (u->m_size < v->m_size)				return true;
	if (u->m_size > v->m_size)				return false;
	return false;
}

template <typename T>
bool isLess_OrderingCompositionAlgorithmSize(OneTestResult<T>* u, OneTestResult<T>* v) {
	//	compare orderings
	if (u->m_ordering < v->m_ordering)		return true;
	if (u->m_ordering > v->m_ordering)		return false;
	//	compare composition
	if (u->m_composition < v->m_composition) return true;
	if (u->m_composition > v->m_composition) return false;
	//	compare algorithm
	if (u->m_algorithm < v->m_algorithm)	return true;
	if (u->m_algorithm > v->m_algorithm)	return false;
	//	compares sizes
	if (u->m_size < v->m_size)				return true;
	if (u->m_size > v->m_size)				return false;
	return false;
}

/*				isSameLine()					*/

template <typename T>
bool isSameLine(OneTestResult<T>* result,
				SortAlgorithms &algorithm,
				ArrayComposition &composition,
				InitialOrdering &ordering)
{
	if (result->m_algorithm 	!= algorithm)		return false;
	if (result->m_composition 	!= composition) 	return false;
	if (!(result->m_ordering  	== ordering))		return false;
	return true;
}

/*				printRowsAlgorithm_ColumnsSize_CellsAverages()		*/

template <typename T>
void printRowPreamble_ColumnsSize_CellsAverages(OneTestResult<T>** results,
												  int num_test_results,
												  ResultTableOrdering& structure) {

	OStreamState ostream_state;	// restores ostream state in destructor

	std::string array_sizes_str = "Array sizes";
	std::string repetitions_str = "Repetitions";

	ArraySizeData size_data;
	getArraySizeData(size_data, results, num_test_results);

	int max_algorithm_strlen 	= getstrlenAlgorithm(results, num_test_results);
	int max_composition_strlen 	= getstrlenComposition(results, num_test_results);
	int max_ordering_strlen 	= getstrlenOrdering(results, num_test_results);
	int max_digits_compares 	= getMaxDigitsCompares(results, num_test_results);
	int max_digits_moves 		= getMaxDigitsAssignments(results, num_test_results);
	int max_digits_repetitions	= getMaxDigitsRepetitions(results, num_test_results);
	int compares_strlen		= max_digits_compares;
	int assignments_strlen	= max_digits_moves;
	int repetitions_strlen	= max_digits_repetitions;

	//	Ensure compares occupy at least the width of the "avg cmp" string
	if (compares_strlen < average_compares_strlen) {
		compares_strlen = average_compares_strlen;
	}
	//	Ensure assignments occupy at least the width of the "avg asgn" string
	if (assignments_strlen < average_assignments_strlen) {
		assignments_strlen = average_assignments_strlen;
	}

	int array_size_header_width =
			max_algorithm_strlen 	+ space_separator_strlen +
			max_composition_strlen 	+ space_separator_strlen +
			max_ordering_strlen		+ space_separator_strlen +
			repetitions_strlen 		+ colon_separator_strlen;

	//	header row:  " Array_sizes:    8      16      32   16384  "
	std::cout 	<< std::setw(array_size_header_width) << std::right
				<< array_sizes_str
				<< space_separator;
	for (int i = 0; i != size_data.num_sizes; i++) {
		std::cout << std::setw(compares_strlen + slash_separator_strlen)
				  << size_data.sizes[i];
		std::cout << std::setw(assignments_strlen) << "" << space_separator;
	}
	std::cout << std::endl;

	//	second row:  "   repetitions  avg cmp / avg asgn     avg cmp / avg asgn "
	int cmp_vs_asgn_extra_space = 0;
	std::cout 	<< std::setw(array_size_header_width) << std::right
				<< repetitions_str
				<< space_separator;
	for (int i = 0; i != size_data.num_sizes; i++) {
		std::cout << std::setw(compares_strlen) 		<< std::right << average_compares_string
				  << std::setw(cmp_vs_asgn_extra_space) << slash_separator
				  << std::setw(assignments_strlen) 		<< std::left << average_assignments_string
				  << space_separator
				  << std::setw(stability_string_strlen) << std::left << stability_string
				  << space_separator;
	}
	std::cout << std::endl;

	SortAlgorithms 		current_algorithm;
	ArrayComposition 	current_composition;
	InitialOrdering 	current_ordering;
	SortAlgorithms		previous_algorithm;
	ArrayComposition	previous_composition;
	InitialOrdering		previous_ordering;

	bool m_ignore_was_announced = false;

	for (int i = 0; i != num_test_results; i++) {
		//	if new grouping, print the row preamble
		if(i == 0 || !isSameLine(results[i],
								 current_algorithm,
								 current_composition,
								 current_ordering)) {
			current_algorithm 	= results[i]->m_algorithm;
			current_composition = results[i]->m_composition;
			current_ordering 	= results[i]->m_ordering;
			//	if previous_? have previously been set
			if (i != 0) {
				//	put the line feed at the end of the previous line
				std::cout << std::endl;
				//	if this line is also the end of a block,
				//	line feed a blank line
				switch(structure.block_end()) {
				default:
				case ResultTableElements::ALGORITHM:
					if (current_algorithm != previous_algorithm)
						std::cout << std::endl;
					break;
				case ResultTableElements::COMPOSITION:
					if (current_composition != previous_composition)
						std::cout << std::endl;
					break;
				case ResultTableElements::ORDERING:
					if (current_ordering != previous_ordering)
						std::cout << std::endl;
					break;
				}
				m_ignore_was_announced 	= false;
			}
			//	Print the row preamble including algorithm, comp & ordering
			std::cout << rowPreambleToString(
							current_algorithm, current_composition, current_ordering,
							max_algorithm_strlen, max_composition_strlen, max_ordering_strlen,
							structure)
					  << space_separator;
			//	Print the number of repetitions of this test result
			std::cout << std::setw(repetitions_strlen) << std::right
					  <<  results[i]->m_sort_metrics.num_repetitions
					  << space_separator;
			previous_algorithm		= current_algorithm;
			previous_composition	= current_composition;
			previous_ordering		= current_ordering;
		}
		if (results[i]->m_ignore) {
			if (!m_ignore_was_announced) {
				std::cout << " result[i].m_ignore is true, no result reported";
				m_ignore_was_announced = true;
			}
			continue;
		}
		//	print out the next set of test metrics
		std::cout << std::setw(compares_strlen) << std::right
				  << results[i]->m_sort_metrics.compares_str()
				  << std::setw(0) << slash_separator
				  << std::setw(assignments_strlen) << std::right
				  << results[i]->m_sort_metrics.assignments_str()
				  << space_separator
				  << std::setw(stability_string_strlen) << std::right
				  << stabilityToString(results[i]->m_composition.composition,
						  	  	  	   results[i]->m_is_stable)
				  << space_separator;
	}
	std::cout << std::endl << std::endl;

	if (size_data.sizes) {
		delete size_data.sizes;
		size_data.sizes = nullptr;
	}
}


/*				printTestResults()					*/


template <typename T>
void printTestResults(OneTestResult<T> **results, int num_test_results,
					  ResultTableOrdering& structure) {

	std::cout << test_result_table_header << std::endl
			  << "Sort Test Results" << std::endl
			  << test_result_table_header << std::endl;
	CompareFunction<T> isLess = assignCompareFunction<T>(structure);
	sortResultsArray(results, num_test_results, isLess);
	printRowPreamble_ColumnsSize_CellsAverages(results, num_test_results, structure);
}

/*
 *	bubble sort with  !was_swap  optimization
 */
template <typename T>
void sortResultsArray(OneTestResult<T>** results, int num_tests,
					  CompareFunction<T> isULessThanV) {

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
