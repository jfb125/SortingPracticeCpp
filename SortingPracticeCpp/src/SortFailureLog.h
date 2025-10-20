/*
 * SortFailureLog.h
 *
 *  Created on: Jun 17, 2025
 *      Author: joe
 */

#ifndef SORTFAILURELOG_H_
#define SORTFAILURELOG_H_

#include <iostream>
#include <iomanip>
#include <sstream>

//#include "SortingDataType.h"
#include "SortingDataTypes.h"

template <typename T>
class SortFailureLog {

public:
	IsSortedResult _diagnostics;
	T* _input;
	T* _result;
	std::string *_message;


	std::string to_string(void) const {
		std::stringstream retval;

		if (_diagnostics._is_sorted) {
			retval << "    no failure occurred" << std::endl;
		} else {
			retval << "!!!!!!!!!! **** failure occurred **** !!!!!!!!!!" << std::endl;
			if ( _result) {
				retval	<< "  result: " << std::endl
						<< "  " << _result->dump() << std::endl;
				retval 	<< "  elements out-of-place were " << std::endl
						<< "  [" << _diagnostics._mismatched_index_i<< "] "
						<< *(*_result)[_diagnostics._mismatched_index_i]
						<< std::endl
						<< "   vs "
						<< std::endl
						<< "  [" << _diagnostics._mismatched_index_j<< "] "
						<< *(*_result)[_diagnostics._mismatched_index_j]
					    << std::endl;
			} else {
				retval 	<< "  result: <nullptr>" << std::endl;
				retval 	<< "  elements out-of-place were ["
						<< _diagnostics._mismatched_index_i
						<< "] vs ["
						<< _diagnostics._mismatched_index_j << "]"
						<< std::endl;
			}
		}

		if (_message) {
			retval << "  " << *_message;
		} else {
			retval << "   (no failure log messages)";
		}

		return retval.str();
	}

	void clear(void)
	{

		if (_input) {
			delete _input;
			_input = nullptr;
		}
		if (_result) {
			delete _result;
			_result = nullptr;
		}
		if (_message) {
			delete _message;
			_message = nullptr;
		}
		_diagnostics._is_sorted = false;
		_diagnostics._mismatched_index_i = 0;
		_diagnostics._mismatched_index_j = 0;
	}


	SortFailureLog() {
		_diagnostics.clear();
		_input = nullptr;
		_result = nullptr;
		_message = nullptr;
	}
	virtual ~SortFailureLog() {
		if (_input) {
			delete _input;
			_input = nullptr;
		}
		if (_result) {
			delete _result;
			_result = nullptr;
		}
		if (_message) {
			delete _message;
			_message = nullptr;
		}
		_diagnostics.clear();
	}
	// deep copies of pointer objects
	SortFailureLog(IsSortedResult diagnostics,
				   SortingDataType<T> *input,
				   SortingDataType<T> *result,
				   std::string *message)
	{
		_diagnostics = diagnostics;
		if (input) {
			_input = input;
		} else {
			_input = nullptr;
		}
		if (result) {
			_result = nullptr;
		} else {
			_result = nullptr;
		}
		if (message) {
			_message = new std::string(*message);
		} else {
			_message = nullptr;
		}
	}

	SortFailureLog(const SortFailureLog &other)
	{
		if (other._input) {
			_input = nullptr;
		} else {
			_input = nullptr;
		}
		if (other._result) {
			_result = nullptr;
		} else {
			_result = nullptr;
		}
		if (other._message) {
			_message = new std::string(*other._message);
		} else {
			_message = nullptr;
		}
		_diagnostics = other._diagnostics;
	}

	SortFailureLog& operator=(const SortFailureLog &other) {

		if (this != &other) {
			if (other._input) {
				_input = nullptr;
			} else {
				_input = nullptr;
			}
			if (other._result) {
				_result = nullptr;
			} else {
				_result = nullptr;
			}
			if (other._message) {
				_message = new std::string(*other._message);
			} else {
				_message = nullptr;
			}
			_diagnostics = other._diagnostics;
		}
	}

	SortFailureLog(SortFailureLog &&other) noexcept
	{
		_input = other._input;
		other._input = nullptr;
		_result = other._result;
		other._result = nullptr;
		_message = other._message;
		other._message = nullptr;
		_diagnostics = other._diagnostics;
	}

	SortFailureLog& operator=(SortFailureLog &&other) noexcept {
		if (this != &other) {
			_input = other._input;
			other._input = nullptr;
			_result = other._result;
			other._result = nullptr;
			_message = other._message;
			other._message = nullptr;
			_diagnostics = other._diagnostics;
		}
		return *this;
	}
}
;
template <typename T>
std::ostream& operator<<(std::ostream &out, const SortFailureLog<T> &log) {
	out << log.to_string();
	return out;
}

#endif /* SORTFAILURELOG_H_ */
