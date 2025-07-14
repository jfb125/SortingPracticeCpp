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

typedef enum ResultOutputPositions OutputParameterFormatSpecifier;

#define DEFAULT_ALGORITHM_FORMAT_SPECIFIER		ResultOutputPositions::DO_NOT_PRINT
#define DEFAULT_COMPOSITION_FORMAT_SPECIFIER	ResultOutputPositions::DO_NOT_PRINT
#define DEFAULT_ORDERING_FORMAT_SPECIFIER		ResultOutputPositions::DO_NOT_PRINT
#define DEFAULT_SORT_FORMAT_SPECIFIER			ResultOutputPositions::DO_NOT_PRINT

#define ARRAY_SIZE_COLUMN_WIDTH 20

class ResultOutputFormatSpecifiers {
private:
	OutputParameterFormatSpecifier m_algorithm;
	OutputParameterFormatSpecifier m_composition;
	OutputParameterFormatSpecifier m_ordering;
	OutputParameterFormatSpecifier m_sort;
public:

	OutputParameterFormatSpecifier algorithm(void) const;
	OutputParameterFormatSpecifier& algorithm(OutputParameterFormatSpecifier&);
	OutputParameterFormatSpecifier composition(void) const;
	OutputParameterFormatSpecifier& composition(OutputParameterFormatSpecifier&);
	OutputParameterFormatSpecifier ordering(void) const;
	OutputParameterFormatSpecifier& ordering(OutputParameterFormatSpecifier&);
	OutputParameterFormatSpecifier sort(void) const;
	OutputParameterFormatSpecifier& sort(OutputParameterFormatSpecifier&);

	ResultOutputFormatSpecifiers();
	ResultOutputFormatSpecifiers(
			OutputParameterFormatSpecifier alg,
			OutputParameterFormatSpecifier comp,
			OutputParameterFormatSpecifier ord,
			OutputParameterFormatSpecifier sort);
	ResultOutputFormatSpecifiers(const ResultOutputFormatSpecifiers&);
	ResultOutputFormatSpecifiers& operator=(const ResultOutputFormatSpecifiers&);
};

#define PRINT_TEST_RESULT_TABLE_HEADER "===================================="

void printTestResults(  OneTestResult** results, int num_test_runs);

void terseDump(OneTestResult** results, int num_test_results);

#endif /* RESULTOUTPUTPARAMETERS_H_ */
