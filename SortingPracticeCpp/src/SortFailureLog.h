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
	T* input_array;
	T* result_array;
	std::string *_message;


	std::string to_string(void) const {
		std::stringstream retval;

		if (_diagnostics._is_sorted) {
			retval << "    no failure occurred" << std::endl;
		} else {
			retval << "!!!!!!!!!! **** failure occurred **** !!!!!!!!!!" << std::endl;
			if ( result_array) {
				retval	<< "  result: " << std::endl
						<< "  " << result_array->dump() << std::endl;
				retval 	<< "  elements out-of-place were " << std::endl
						<< "  [" << _diagnostics._mismatched_index_i<< "] "
						<< (*result_array)[_diagnostics._mismatched_index_i]
						<< std::endl
						<< "   vs "
						<< std::endl
						<< "  [" << _diagnostics._mismatched_index_j<< "] "
						<< (*result_array)[_diagnostics._mismatched_index_j]
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

		if (input_array) {
			delete input_array;
			input_array = nullptr;
		}
		if (result_array) {
			delete result_array;
			result_array = nullptr;
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
		input_array 	= nullptr;
		result_array 	= nullptr;
		_message 		= nullptr;
	}
	virtual ~SortFailureLog() {
		if (input_array) {
			delete input_array;
			input_array = nullptr;
		}
		if (result_array) {
			delete result_array;
			result_array = nullptr;
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
			input_array = input;
		} else {
			input_array = nullptr;
		}
		if (result) {
			result_array = nullptr;
		} else {
			result_array = nullptr;
		}
		if (message) {
			_message = new std::string(*message);
		} else {
			_message = nullptr;
		}
	}

	SortFailureLog(const SortFailureLog &other)
	{
		if (other.input_array) {
			input_array = nullptr;
		} else {
			input_array = nullptr;
		}
		if (other.result_array) {
			result_array = nullptr;
		} else {
			result_array = nullptr;
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
			if (other.input_array) {
				input_array = nullptr;
			} else {
				input_array = nullptr;
			}
			if (other.result_array) {
				result_array = nullptr;
			} else {
				result_array = nullptr;
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
		input_array 		= other.input_array;
		other.input_array 	= nullptr;
		result_array 		= other.result_array;
		other.result_array 	= nullptr;
		_message = other._message;
		other._message 		= nullptr;
		_diagnostics 		= other._diagnostics;
	}

	SortFailureLog& operator=(SortFailureLog &&other) noexcept {
		if (this != &other) {
			input_array 		= other.input_array;
			other.input_array 	= nullptr;
			result_array 		= other.result_array;
			other.result_array 	= nullptr;
			_message 			= other._message;
			other._message 		= nullptr;
			_diagnostics 		= other._diagnostics;
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
