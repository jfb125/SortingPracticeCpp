/*
 *
 *  Created on: Apr 18, 2025
 *      Author: joe
 */

//#include "OneTestResult.h"

/* ************************************************	*/
/*	this is private to prevent a user from creating	*/
/*	 an object without a size_results array			*/
/* ************************************************	*/
#if 0
OneTestResult::OneTestResult() {

	_algorithm = SortAlgorithms::SORT_ALGORITHMS_COUNT;
	_composition._composition = DEFAULT_ARRAY_COMPOSITION;
	_composition._num_different = 0;
	_composition._num_discrete_values = 0;
	_ordering = InitialOrderings::INITIAL_ORDERINGS_COUNT;
	_failure_log = new SortFailureLog();
	_size = 0;
	_sort_metrics.compares = 0;
	_sort_metrics.assignments = 0;
	_sort_metrics._num_repetitions = 0;
	_messages = new MessageList();
}
#endif

/* ************************************************	*/
/*				public constructors					*/
/* ************************************************	*/
#if 0
OneTestResult::OneTestResult(SortAlgorithms algorithm,
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
	_failure_log = new SortFailureLog();
	_messages = new MessageList();
}
#endif
#if 0
//	need to do a deep copy of both _failure_log & _messages*
OneTestResult::OneTestResult(OneTestResult &other) {

	if (this != &other) {
		_algorithm = other._algorithm;
		_composition = other._composition;
		_ordering = other._ordering;
		_size = other._size;
		_sort_metrics = other._sort_metrics;

		if (other._failure_log)
			// if other has an object copy it
			_failure_log = new SortFailureLog(*other._failure_log);
		else
			// else create a default object
			_failure_log = new SortFailureLog();

		if (other._messages)
			// if other has an object, copy it
			_messages = new MessageList(*other._messages);
		else
			// else create a default object
			_messages = new MessageList();
	}
}
#endif
#if 0
OneTestResult& OneTestResult::operator=(OneTestResult &other) {
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
			_failure_log = new SortFailureLog(*other._failure_log);
		} else {
			// else create a default object
			_failure_log = new SortFailureLog();
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
#endif
#if 0
OneTestResult::OneTestResult(OneTestResult &&other) noexcept {

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
			_failure_log = new SortFailureLog();
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
#endif
#if 0
OneTestResult::~OneTestResult() {

	if (_failure_log) {
		delete _failure_log;
		_failure_log = nullptr;
	}
	if (_messages) {
		delete _messages;
		_messages = nullptr;
	}
}
#endif
#if 0
OneTestResult& OneTestResult::operator=(OneTestResult &&other) noexcept {

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
			_failure_log = new SortFailureLog();
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
#endif
#if 0
OneTestResult& OneTestResult::operator+=(char *char_array) {

	_messages->enqueue(char_array);
	return *this;
}
OneTestResult& OneTestResult::operator+=(const std::string &string_to_add) {

	_messages->enqueue(string_to_add);
	return *this;
}
#endif
/*	**********************************************************	*/
/*					non member functions						*/
/*	**********************************************************	*/
#if 0
std::string OneTestResult::str(void) const {

	std::stringstream result;

	//	avoid std::left and std::right persisting after a call to printStudentList()
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
#endif
#if 0
std::ostream& operator<<(std::ostream &out, OneTestResult& object) {

	out << object.str();
	return out;
}
#endif
