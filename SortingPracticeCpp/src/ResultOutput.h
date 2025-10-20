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

#define PRINT_TEST_RESULT_TABLE_HEADER "===================================="
template <typename T>
void terseDump(OneTestResult<T> *result, int num_test_results) {
	std::cout << __FUNCTION__ << " stubbed out\n";
}
template <typename T>
void printTestResults(OneTestResult<T> **results, int num_test_results) {

	std::cout << __FUNCTION__ << " stubbed out\n";
//	// print the title of the table
//	std::cout << test_result_table_header << std::endl
//			  << "Sort Test Results" << std::endl
//			  << test_result_table_header << std::endl;
//	bool (*isLess)(OneTestResult* u, OneTestResult*v);
//	isLess = isLess_CompostionOrderingAlgorithmSize;
//	printRowsAlgorithm_ColumnsSize_CellsAverages(results, num_test_results, isLess);
}


#endif /* RESULTOUTPUTPARAMETERS_H_ */
