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

enum class TestResultMember{
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

bool isValid(TestResultMember object);
TestResultMember& operator++(TestResultMember& value);
TestResultMember  operator++(TestResultMember &value, int unused);
bool operator==(TestResultMember u, TestResultMember v);
bool operator<(TestResultMember u, TestResultMember v);
bool operator<=(TestResultMember u, TestResultMember v);
bool operator>(TestResultMember u, TestResultMember v);
bool operator>=(TestResultMember u, TestResultMember v);
bool operator!=(TestResultMember u, TestResultMember v);

constexpr int test_parameter_label_width 	= 14;
constexpr int test_algorithm_string_width 	= 1;
constexpr int test_composition_string_width	= 1;
constexpr int test_initial_ordering_width	= 1;
constexpr int test_array_size_width			= 10;


template <typename T>
class OneTestResult {
private:
	// don't allow a constructor that does not
	//	allocate the array size_results
	OneTestResult() {
		m_algorithm 					= SortAlgorithms::SORT_ALGORITHMS_COUNT;
		m_composition.composition 		= DEFAULT_ARRAY_COMPOSITION;
		m_composition.num_different 	= 0;
		m_composition.num_distinct_values = 0;
		m_ordering 						= InitialOrdering();
		m_failure_log 					= new SortFailureLog<T>();
		m_size 							= 0;
		m_sort_metrics.compares 		= 0;
		m_sort_metrics.assignments 		= 0;
		m_sort_metrics.num_repetitions = 0;
		m_messages = new MessageList();
	}

public:
	SortAlgorithms 		m_algorithm;
	ArrayComposition 	m_composition;
	InitialOrdering 	m_ordering;
	array_size_t 		m_size;
	SortTestMetrics 	m_sort_metrics;
	SortFailureLog<T> 	*m_failure_log;

	MessageList 		*m_messages;

	std::string str(void) const;

	OneTestResult(SortAlgorithms 	x_algorithm,
				  ArrayComposition 	x_composition,
				  InitialOrdering 	x_ordering,
				  array_size_t 		x_size,
				  num_repetitions_t x_num_repetitions) {

		m_algorithm 					= x_algorithm;
		m_composition 					= x_composition;
		m_ordering 						= x_ordering;
		m_size 							= x_size;
		m_sort_metrics.compares 		= 0;
		m_sort_metrics.assignments 		= 0;
		m_sort_metrics.num_repetitions = x_num_repetitions;
		m_failure_log 	= new SortFailureLog<T>();
		m_messages 		= new MessageList();
	}

	OneTestResult(OneTestResult &other) {
		if (this != &other) {
			m_algorithm 	= other.m_algorithm;
			m_composition 	= other.m_composition;
			m_ordering 		= other.m_ordering;
			m_size 			= other.m_size;
			m_sort_metrics 	= other.m_sort_metrics;

			if (other.m_failure_log)
				// if other has an object copy it
				m_failure_log = new SortFailureLog<T>(*other.m_failure_log);
			else
				// else create a default object
				m_failure_log = new SortFailureLog<T>();

			if (other.m_messages)
				// if other has an object, copy it
				m_messages = new MessageList(*other.m_messages);
			else
				// else create a default object
				m_messages = new MessageList();
		}
	}

	OneTestResult& operator=(OneTestResult &other) {
		if (this != &other) {
			m_algorithm 	= other.m_algorithm;
			m_composition 	= other.m_composition;
			m_ordering 		= other.m_ordering;
			m_size 			= other.m_size;
			m_sort_metrics 	= other.m_sort_metrics;

			// delete existing object
			if (m_failure_log) {
				delete m_failure_log;
			}
			if (other.m_failure_log) {
				// if other has an object, copy it
				m_failure_log = new SortFailureLog<T>(*other.m_failure_log);
			} else {
				// else create a default object
				m_failure_log = new SortFailureLog<T>();
			}

			// delete existing object
			if (m_messages) {
				delete m_messages;
			}
			if (other.m_messages) {
				// if other has an object, copy it
				m_messages = new MessageList(*other.m_messages);
			} else {
				// else create a default object
				m_messages = new MessageList();
			}
		}
		return *this;
	}

	OneTestResult(OneTestResult&& other) noexcept {
		if (this != &other) {
			m_algorithm 	= other.m_algorithm;
			m_composition 	= other.m_composition;
			m_ordering 		= other.m_ordering;
			m_size 			= other.m_size;
			m_sort_metrics 	= other.m_sort_metrics;

			if (m_failure_log) {
				delete m_failure_log;
				m_failure_log = nullptr;
			}
			if (other.m_failure_log) {
				// take ownership of other's object
				m_failure_log 		= other.m_failure_log;
				other.m_failure_log = nullptr;
			} else {
				// create a default object
				m_failure_log 		= new SortFailureLog<T>();
			}

			if (m_messages) {
				delete m_messages;
				m_messages = nullptr;
			}
			if (other.m_messages) {
				// take ownership of other's object
				m_messages 		= other.m_messages;
				other.m_messages= nullptr;
			} else {
				// create a default object
				m_messages = new MessageList();
			}
		}
	}

	OneTestResult& operator=(OneTestResult&& other) noexcept {

		if (this != &other) {
			// copy other's non-dynamically allocated members
			m_algorithm 		= other.m_algorithm;
			m_composition 		= other.m_composition;
			m_ordering 			= other.m_ordering;
			m_sort_metrics 		= other.m_sort_metrics;

			// delete *this object
			if (m_failure_log) {
				delete m_failure_log;
				m_failure_log = nullptr;
			}
			if (other.m_failure_log) {
				// take ownership of other dynamically allocated structure
				m_failure_log 		= other.m_failure_log;
				other.m_failure_log = nullptr;
			} else {
				// create a default object
				m_failure_log 		= new SortFailureLog<T>();
			}

			// delete *this object
			if (m_messages) {
				delete m_messages;
				m_failure_log 	= nullptr;
			}
			if (other.m_messages) {
				// take ownership of other dynamically allocated object
				m_messages 		= other.m_messages;
				other.m_messages= nullptr;
			} else {
				// other is nullptr, create a default object
				m_messages 	= new MessageList();
			}
		}
		return *this;
	}

	~OneTestResult() {
		if (m_failure_log) {
			delete m_failure_log;
			m_failure_log = nullptr;
		}
		if (m_messages) {
			delete m_messages;
			m_messages = nullptr;
		}
	}

	OneTestResult& operator+=(char *char_array) {

		m_messages->enqueue(char_array);
		return *this;
	}
	OneTestResult& operator+=(const std::string &string_to_add) {

		m_messages->enqueue(string_to_add);
		return *this;
	}

	std::string to_string() {

		OStreamState ostream_state;	// restores ostream state in it's destructor
		std::stringstream result;

		result 	<< std::setw(test_parameter_label_width)
				<< std::left << "algorithm:"
				<< std::setw(test_algorithm_string_width)
				<< std::left << m_algorithm;
		result	<< std::endl;
		result	<< std::setw(test_parameter_label_width)
				<< std::left << "composition:"
				<< std::setw(test_composition_string_width)
				<< std::left << m_composition;
		result	<< std::endl;
		result 	<< std::setw(test_parameter_label_width)
				<< std::left << "ordered:"
				<< std::setw(test_initial_ordering_width)
				<< std::left << m_ordering;
		result	<< std::endl;
		result	<< std::setw(test_parameter_label_width)
				<< std::left << "array size:"
				<< std::setw(test_array_size_width)
				<< std::left << m_size;
		result	<< std::endl;
		result 	<< std::setw(test_parameter_label_width)
				<< std::left << "metrics:"
				<< std::endl
				<< m_sort_metrics.averagesStr();
		result  << std::endl;

		if (m_failure_log) {
			result << *m_failure_log << std::endl;
		}

		if (m_messages && !m_messages->isEmpty()) {
			int i = 0;
			int stop = m_messages->size() - 1;
			//	print all messages except the last on separate lines
			for ( ; i < stop; i++) {
				result << " message " << i << ": "
					   << m_messages->peek(i) << std::endl;
			}
			// print the last message without a line feed
			if (i < m_messages->size()) {
				result << " message " << i << ": "
					   << m_messages->peek(i);
			}
		} else {
			result << "(no test result messages)";
		}
		result << std::endl;

		return result.str();
	}

	std::ostream& operator<<(std::ostream &out) {
		out = this->to_string();
		return out;
	}
};


#endif /* OneTestResult_H_ */
