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
#include "OStreamState.h"

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

#define _TEST_PARAMETER_LABEL_WIDTH		14
#define _TEST_ALGORITHM_STRING_WIDTH 	1
#define _TEST_COMPOSITION_STRING_WIDTH 	1
#define _TEST_INITIAL_ORDERING_WIDTH 	1
#define _TEST_ARRAY_SIZE_WIDTH 			10


template <typename T>
class OneTestResult {
private:
	// don't allow a constructor that does not
	//	allocate the array size_results
	OneTestResult() {
		_algorithm 					= SortAlgorithms::SORT_ALGORITHMS_COUNT;
		_composition._composition 	= DEFAULT_ARRAY_COMPOSITION;
		_composition._num_different = 0;
		_composition._num_discrete_values = 0;
		_ordering 					= InitialOrderings::INITIAL_ORDERINGS_COUNT;
		_failure_log 				= new SortFailureLog<T>();
		_size 						= 0;
		_sort_metrics.compares 		= 0;
		_sort_metrics.assignments 	= 0;
		_sort_metrics._num_repetitions = 0;
		_messages = new MessageList();
	}

public:
	SortAlgorithms 		_algorithm;
	ArrayComposition 	_composition;
	InitialOrdering 	_ordering;
	array_size_t 		_size;
	SortTestMetrics 	_sort_metrics;
	SortFailureLog<T> 	*_failure_log;

	MessageList 		*_messages;

//	OneTestResult& operator+=(char *);
//	OneTestResult& operator+=(const std::string&);

	std::string str(void) const;

	OneTestResult(SortAlgorithms algorithm,
								 ArrayComposition composition,
								 InitialOrdering ordering,
								 array_size_t size,
								 num_repetitions_t num_repetitions) {

		_algorithm = algorithm;
		_composition = composition;
		_ordering = ordering;
		_size = size;
		_sort_metrics.compares = 0;
		_sort_metrics.assignments = 0;
		_sort_metrics._num_repetitions = num_repetitions;
		_failure_log = new SortFailureLog<T>();
		_messages = new MessageList();
	}

	OneTestResult(OneTestResult &other) {
		if (this != &other) {
			_algorithm = other._algorithm;
			_composition = other._composition;
			_ordering = other._ordering;
			_size = other._size;
			_sort_metrics = other._sort_metrics;

			if (other._failure_log)
				// if other has an object copy it
				_failure_log = new SortFailureLog<T>(*other._failure_log);
			else
				// else create a default object
				_failure_log = new SortFailureLog<T>();

			if (other._messages)
				// if other has an object, copy it
				_messages = new MessageList(*other._messages);
			else
				// else create a default object
				_messages = new MessageList();
		}
	}

	OneTestResult& operator=(OneTestResult &other) {
		if (this != &other) {

			_algorithm = other._algorithm;
			_composition = other._composition;
			_ordering = other._ordering;
			_size = other._size;
			_sort_metrics = other._sort_metrics;

			// delete existing object
			if (_failure_log) {
				delete _failure_log;
			}
			if (other._failure_log) {
				// if other has an object, copy it
				_failure_log = new SortFailureLog<T>(*other._failure_log);
			} else {
				// else create a default object
				_failure_log = new SortFailureLog<T>();
			}

			// delete existing object
			if (_messages) {
				delete _messages;
			}
			if (other._messages) {
				// if other has an object, copy it
				_messages = new MessageList(*other._messages);
			} else {
				// else create a default object
				_messages = new MessageList();
			}
		}
		return *this;
	}

	OneTestResult(OneTestResult&& other) noexcept {
		if (this != &other) {
			_algorithm = other._algorithm;
			_composition = other._composition;
			_ordering = other._ordering;
			_size = other._size;
			_sort_metrics = other._sort_metrics;

			if (other._failure_log) {
				// take ownership of other's object
				_failure_log = other._failure_log;
				other._failure_log = nullptr;
			} else {
				// create a default object
				_failure_log = new SortFailureLog<T>();
			}

			if (other._messages) {
				// take ownership of other's object
				_messages = other._messages;
				other._messages = nullptr;
			} else {
				// create a default object
				_messages = new MessageList();
			}
		}
	}

	OneTestResult& operator=(OneTestResult&& other) noexcept {

		if (this != &other) {
			// copy other's non-dynamically allocated members
			_algorithm = other._algorithm;
			_composition = other._composition;
			_ordering = other._ordering;
			_sort_metrics = other._sort_metrics;

			// delete *this object
			if (_failure_log) {
				delete _failure_log;
				_failure_log = nullptr;
			}
			if (other._failure_log) {
				// take ownership of other dynamically allocated structure
				_failure_log = other._failure_log;
				other._failure_log = nullptr;
			} else {
				// create a default object
				_failure_log = new SortFailureLog<T>();
			}

			// delete *this object
			if (_messages) {
				delete _messages;
				_failure_log = nullptr;
			}
			if (other._messages) {
				// take ownership of other dynamically allocated object
				_messages = other._messages;
				other._messages = nullptr;
			} else {
				// other is nullptr, create a default object
				_messages = new MessageList();
			}
		}
		return *this;
	}

	~OneTestResult() {
		if (_failure_log) {
			delete _failure_log;
			_failure_log = nullptr;
		}
		if (_messages) {
			delete _messages;
			_messages = nullptr;
		}
	}

	OneTestResult& operator+=(char *char_array) {

		_messages->enqueue(char_array);
		return *this;
	}
	OneTestResult& operator+=(const std::string &string_to_add) {

		_messages->enqueue(string_to_add);
		return *this;
	}

	std::string to_string() {
		std::stringstream result;

		OStreamState ostream_start;

		std::ios_base::fmtflags _flags = std::cout.flags();

		result 	<< std::setw(_TEST_PARAMETER_LABEL_WIDTH) << std::left << "algorithm:"
				<< std::setw(_TEST_ALGORITHM_STRING_WIDTH) << std::left << _algorithm;
		result	<< std::endl;
		result	<< std::setw(_TEST_PARAMETER_LABEL_WIDTH) << std::left << "composition:"
				<< std::setw(_TEST_COMPOSITION_STRING_WIDTH) << std::left << _composition;
		result	<< std::endl;
		result 	<< std::setw(_TEST_PARAMETER_LABEL_WIDTH) << std::left << "ordered:"
				<< std::setw(_TEST_INITIAL_ORDERING_WIDTH) << std::left << _ordering;
		result	<< std::endl;
		result	<< std::setw(_TEST_PARAMETER_LABEL_WIDTH) << std::left << "array size:"
				<< std::setw(_TEST_ARRAY_SIZE_WIDTH) << std::left << _size;
		result	<< std::endl;
		result 	<< std::setw(_TEST_PARAMETER_LABEL_WIDTH) << std::left << "metrics:"
				<< std::endl
	//			<< _sort_metrics.totalCounts() << ", "
				<< _sort_metrics.averagesStr();
		result  << std::endl;

		if (_failure_log) {
			result << *_failure_log << std::endl;
		}

		if (_messages && !_messages->isEmpty()) {
			int i = 0;
			int stop = _messages->size() - 1;
			//	print all messages except the last on separate lines
			for ( ; i < stop; i++) {
				result << " message " << i << ": " << _messages->peek(i) << std::endl;
			}
			// print the last message without a line feed
			if (i < _messages->size()) {
				result << " message " << i << ": " << _messages->peek(i);
			}
		} else {
			result << "(no test result messages)";
		}
		result << std::endl;

		std::cout.flags(_flags);
		return result.str();
	}

	std::ostream& operator<<(std::ostream &out) {
		out = this->to_string();
		return out;
	}
};


#endif /* OneTestResult_H_ */
