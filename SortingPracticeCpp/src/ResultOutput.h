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

enum ResultOutputPositions {
	DO_NOT_PRINT,
	TABLE_ROWS,
	TABLE_COLUMNS,
	TABLE_TITLE
};

typedef enum ResultOutputPositions ResultParameterFormat;

#define DEFAULT_ALGORITHM_FORMAT_SPECIFIER		ResultOutputPositions::DO_NOT_PRINT
#define DEFAULT_COMPOSITION_FORMAT_SPECIFIER	ResultOutputPositions::DO_NOT_PRINT
#define DEFAULT_ORDERING_FORMAT_SPECIFIER		ResultOutputPositions::DO_NOT_PRINT
#define DEFAULT_SORT_FORMAT_SPECIFIER			ResultOutputPositions::DO_NOT_PRINT

#define ARRAY_SIZE_COLUMN_WIDTH 20

class ResultFormat {
private:
	ResultParameterFormat m_algorithm;
	ResultParameterFormat m_composition;
	ResultParameterFormat m_ordering;
	ResultParameterFormat m_sort;
public:

	ResultParameterFormat algorithm(void) const {
		return m_algorithm;
	}
	ResultParameterFormat& algorithm(const ResultParameterFormat &algo) {
		m_algorithm = algo;
		return m_algorithm;
	}
	ResultParameterFormat composition(void) const {
		return m_composition;
	}
	ResultParameterFormat& composition(const ResultParameterFormat &comp) {
		m_composition = comp;
		return m_composition;

	}
	ResultParameterFormat ordering(void) const {
		return m_ordering;
	}
	ResultParameterFormat& ordering(const ResultParameterFormat &ordrng) {
		m_ordering = ordrng;
		return m_ordering;
	}
	ResultParameterFormat sort(void) const {
		return m_sort;
	}
	ResultParameterFormat& sort(const ResultParameterFormat &srt) {
		m_sort = srt;
		return m_sort;
	}

	ResultFormat();
	ResultFormat( ResultParameterFormat alg,
			ResultParameterFormat comp,
			ResultParameterFormat ord,
			ResultParameterFormat sort);
	ResultFormat(const ResultFormat&);
	ResultFormat& operator=(const ResultFormat&);
};

#define MIN_TEST_RESULT_INTEGER static_cast<int>(MIN_TEST_RESULT_MEMBER)
#define MAX_TEST_RESULT_INTEGER static_cast<int>(MAX_TEST_RESULT_MEMBER)
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

bool isValid(TestResultMember object);

TestResultMember& operator++(TestResultMember&);
TestResultMember  operator++(TestResultMember&, int unused);

bool operator==(TestResultMember u, TestResultMember v);
bool operator<(TestResultMember u, TestResultMember v);
bool operator<=(TestResultMember u, TestResultMember v);
bool operator>(TestResultMember u, TestResultMember v);
bool operator>=(TestResultMember u, TestResultMember v);
bool operator!=(TestResultMember u, TestResultMember v);

template <typename T>
bool isSameLine(OneTestResult<T>* result,
				SortAlgorithms &algorithm,
				ArrayComposition &composition,
				InitialOrdering &ordering)
{
	if (result->_algorithm != algorithm)		return false;
	if (result->_composition != composition) 	return false;
	if (result->_ordering != ordering)			return false;
	return true;
}

void printRowStart(SortAlgorithms &algorithm, ArrayComposition &composition, InitialOrdering &ordering,
					int algorithm_strlen, int composition_strlen, int ordering_strlen);

template <typename T>
void printRowsAlgorithm_ColumnsSize_CellsAverages(OneTestResult<T>** result,
												  int num_test_results,
												  bool (*isLess)(OneTestResult*, OneTestResult*));

//int	getstrlenAlgorithm(OneTestResult ** result, int num_tests);
//int getstrlenComposition(OneTestResult** result, int num_tests);
//int getstrlenOrdering(OneTestResult** result, int num_tests);
//int getMaxDigitsCompares(OneTestResult** result, int num_tests);
//int getMaxDigitsMoves(OneTestResult** result, int num_tests);
//int getMaxDigitsArraySize(OneTestResult** result, int num_tests);
//void getArraySizeData(ArraySizeData &dst, OneTestResult** results, int num_test_results);

template <typename T>
bool isLess_AlgorithmCompostionOrderingSize(OneTestResult<T>* u, OneTestResult<T>* v) {
	//	compare algorithm
	if (u->_algorithm < v->_algorithm)	return true;
	if (u->_algorithm > v->_algorithm)	return false;
	//	compare composition
	if (u->_composition < v->_composition) return true;
	if (u->_composition > v->_composition) return false;
	//	compare orderings
	if (u->_ordering < v->_ordering)	return true;
	if (u->_ordering > v->_ordering)	return false;
	//	compares sizes
	if (u->_size < v->_size)	return true;
	if (u->_size > v->_size)	return false;
	//	the test result parameters are identical
	return false;
}
template <typename T>
bool isLess_CompostionOrderingAlgorithmSize(OneTestResult<T>* u, OneTestResult<T>* v) {
	//	compare composition
	if (u->_composition < v->_composition) return true;
	if (u->_composition > v->_composition) return false;
	//	compare orderings
	if (u->_ordering < v->_ordering)	return true;
	if (u->_ordering > v->_ordering)	return false;
	//	compare algorithm
	if (u->_algorithm < v->_algorithm)	return true;
	if (u->_algorithm > v->_algorithm)	return false;
	//	compares sizes
	if (u->_size < v->_size)	return true;
	if (u->_size > v->_size)	return false;
	//	the test result parameters are identical
	return false;
}

/*
 *	bubble sort with  !was_swap  optimization
 */
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

template <typename T>
void terseDump(OneTestResult<T>** results, int num_test_results) {

	for (int i = 0; i != num_test_results-1; i++) {
		std::cout << std::setw(2) << i << ": "
				  << results[i]->_algorithm << ", "
				  << results[i]->_composition << ", "
				  << results[i]->_ordering << ", "
				  << results[i]->_size
				  << std::endl;
	}
	int i = num_test_results-1;
	std::cout << std::setw(2) << i << ": "
			  << results[i]->_algorithm << ", "
			  << results[i]->_composition << ", "
			  << results[i]->_ordering << ", "
			  << results[i]->_size;
}

template <typename T>
void printTestResults(OneTestResult<T> **results, int num_test_results) {

	std::cout << test_result_table_header << std::endl
			  << "Sort Test Results" << std::endl
			  << test_result_table_header << std::endl;
	bool (*isLess)(OneTestResult<T>* u, OneTestResult<T>*v);
	isLess = isLess_CompostionOrderingAlgorithmSize;
	printRowsAlgorithm_ColumnsSize_CellsAverages(results, num_test_results, isLess);
}


#endif /* RESULTOUTPUTPARAMETERS_H_ */
