/*
 *
 *  Created on: Apr 18, 2025
 *      Author: joe
 */

/*	**************************************************	*/
/*	**************************************************	*/
/*		intended to store the result of running			*/
/*		a given 'algorithm' 'num_repetitions' times		*/
/*		on an array of 'composition' starting each		*/
/*		sort with a given 'initial ordering'			*/
/*	**************************************************	*/
/*	**************************************************	*/

#ifndef ONETESTRESULT_H_
#define ONETESTRESULT_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstring>
#include <climits>

#include "SortingDataTypes.h"
#include "SortAlgorithm.h"
#include "SortFailureLog.h"
#include "ArrayComposition.h"
#include "InitialOrdering.h"
#include "MessageList.h"
#include "SortTestMetrics.h"

enum TestResultMember{
	ALGORITHM,
	ARRAY_COMPOSITION,
	INITIAL_ORDER,
	ARRAY_SIZE,
	SORT_METRICS,
	SORT_FAILURE_LOG
};

#define MIN_TEST_RESULT_MEMBER TestResultMember::ALGORITHM
#define MAX_TEST_RESULT_MEMBER TestResultMember::SORT_FAILURE_LOG
#define NUM_TEST_RESULT_MEMBERS 6

class OneTestResult {
private:
	// don't allow a constructor that does not
	//	allocate the array size_results
	OneTestResult();

public:
	SortAlgorithms _algorithm;
	ArrayComposition _composition;
	InitialOrdering _ordering;
	array_size_t _size;
	SortTestMetrics _sort_metrics;
	SortFailureLog *_failure_log;

	MessageList *_messages;

	OneTestResult& operator+=(char *);
	OneTestResult& operator+=(const std::string&);

	std::string str(void) const;

	OneTestResult(SortAlgorithms, ArrayComposition, InitialOrdering,
				  array_size_t, num_repetitions_t);
	OneTestResult(OneTestResult &);
	OneTestResult& operator=(OneTestResult&);
	OneTestResult(OneTestResult&&) noexcept;
	OneTestResult& operator=(OneTestResult&&) noexcept;
	~OneTestResult();
};

#define _TEST_PARAMETER_LABEL_WIDTH		14
#define _TEST_ALGORITHM_STRING_WIDTH 	1
#define _TEST_COMPOSITION_STRING_WIDTH 	1
#define _TEST_INITIAL_ORDERING_WIDTH 	1
#define _TEST_ARRAY_SIZE_WIDTH 			10

std::ostream& operator<<(std::ostream &out, OneTestResult& object);

#endif /* OneTestResult_H_ */
